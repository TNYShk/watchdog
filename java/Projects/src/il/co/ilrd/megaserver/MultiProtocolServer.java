package il.co.ilrd.megaserver;

import org.omg.PortableInterceptor.ORBInitInfoPackage.DuplicateName;

import java.io.*;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.*;

public class MultiProtocolServer {
    private connectionHandler connectHandler;
    private MessageHandler msgHandler;
    private Set<Integer> portList;
     private volatile boolean isRun;

    public MultiProtocolServer() throws IOException {
        connectHandler = new connectionHandler();
        msgHandler = new MessageHandler();
        portList = new HashSet<>();
    }


    public void addTCP(int port) throws IOException {
        if(port < 0 || port > 60000)
            throw new IllegalArgumentException();
        connectHandler.addTCP(port);
    }

    public void addUDP(int port) throws IOException {
        if(port < 0 || port > 60000)
            throw new IllegalArgumentException();
        connectHandler.addUDP(port);
    }

    public void serverUp(){
        isRun = true;
        Thread srvrUpper = new Thread(() -> {
            try {
                connectHandler.start();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        srvrUpper.start();
    }

    public void serverDown() throws IOException {
        connectHandler.selector.close();
    }

    private class connectionHandler{
        List<SelectionKey> tcpserverList;
        List<SelectionKey> udpserverList;
        private Selector selector;


        public connectionHandler() throws IOException {
            tcpserverList = new ArrayList<>();
            udpserverList = new ArrayList<>();

            selector = Selector.open();
        }
        public void addTCP(int port) throws IOException {
            if(!portList.add(port))
                throw new IllegalArgumentException("duplicate port");
            synchronized (this) {
                ServerSocketChannel tcp = ServerSocketChannel.open();
                tcp.socket().bind(new InetSocketAddress(port));
                tcp.configureBlocking(false);

                SelectionKey sKey = tcp.register(selector, SelectionKey.OP_ACCEPT, tcp);

                tcpserverList.add(sKey);
            }
        }

        public void addUDP(int port) throws IOException {
            if(!portList.add(port))
                throw new IllegalArgumentException("duplicate port");
            synchronized (this) {
                DatagramChannel udp = DatagramChannel.open();
                udp.socket().bind(new InetSocketAddress(port));
                udp.configureBlocking(false);
                SelectionKey sKey = udp.register(selector, SelectionKey.OP_READ, udp);

                udpserverList.add(sKey);
            }
        }

        public void start() throws IOException {
            selector = Selector.open();
            try {
                Iterator<SelectionKey> iter;
                SelectionKey key;
                while (isRun) {
                    selector.select();

                    iter = selector.selectedKeys().iterator();

                    while (iter.hasNext()) {
                        key = iter.next();

                        if (key.attachment() instanceof DatagramChannel) {
                            UDPCommunicator udpConect = new UDPCommunicator();
                            udpConect.handle(key);

                        }
                        else {
                            if (key.isAcceptable()) {
                                ServerSocketChannel ssChannel = (ServerSocketChannel)key.attachment();
                                acceptConnectionTCP tcpAccept = new acceptConnectionTCP(ssChannel);
                                tcpAccept.handle(key);
                            } else if (key.isReadable()) {
                                TCPCommunicator tcpConect = new TCPCommunicator();
                                tcpConect.handle(key);
                            }

                        }
                    iter.remove();
                    }
                }
            }catch (IOException e){
                throw new RuntimeException(e);
            }
        }

        private abstract class Connection{    // need to pass key?
            public abstract void handle(SelectionKey key);
        }

        private abstract class ConnectionCommunicator extends Connection {
            public abstract void send(ByteBuffer buffer);
        }

    private class acceptConnectionTCP extends Connection {
        private SocketChannel client;
        private final ByteBuffer welcomeBuf = ByteBuffer.wrap("TCP communicator handler\n".getBytes());
        private ServerSocketChannel serverSocketChannel;

        public acceptConnectionTCP(ServerSocketChannel serverSocketChannel) {
            super();
            this.serverSocketChannel = serverSocketChannel;
        }


        @Override
        public void handle(SelectionKey key) {
            try {
                client = ((ServerSocketChannel) key.channel()).accept();
                String address = client.socket().getInetAddress().toString() + ":" + client.socket().getPort();
                client.configureBlocking(false);
                client.register(selector, SelectionKey.OP_READ, address);
                client.write(welcomeBuf);
                welcomeBuf.rewind();
                System.out.println("accepted connection from: " + address);

            } catch (ClosedChannelException e) {
                throw new RuntimeException(e);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }
        private class TCPCommunicator extends ConnectionCommunicator {
            private SocketChannel client;
            private ByteBuffer buffer;
            public TCPCommunicator(){
                buffer = ByteBuffer.allocate(1024);
            }
            @Override
        public void handle(SelectionKey key) {
            try {
                client = (SocketChannel) key.channel();
                client.read(buffer);
                buffer.flip();

            }catch(IOException e){
               System.err.println(e);
                throw new RuntimeException();
            }
        }

            @Override
            public void send(ByteBuffer buffer) {
                try {
                    //buffer.rewind();
                    client.write(buffer);
                    buffer.clear();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        private class UDPCommunicator extends ConnectionCommunicator {
            private DatagramChannel client;
            private ByteBuffer buffer;
            private SocketAddress socketAdrsUDP;

            public UDPCommunicator(){
                buffer = ByteBuffer.allocate(1024);
            }

            @Override
            public void handle(SelectionKey key) {
                try {
                    client = (DatagramChannel) key.channel();
                    socketAdrsUDP = client.receive(buffer);
                    buffer.flip();
                    msgHandler.handleMessage(buffer, this);
                }catch(IOException | ClassNotFoundException e){
                    System.err.println(e);
                }
            }

            @Override
            public void send(ByteBuffer buffer) {
                try {
                    client.send(buffer, socketAdrsUDP);
                }catch(IOException e){
                    System.err.println(e);
                }
                finally {
                    buffer.clear();
                }
            }
        }
    }

        private class MessageHandler{
            private Map<ServerProtocol,Protocol> serverProtocol;
            private connectionHandler.ConnectionCommunicator handlerConnectin;

            public MessageHandler(){
                serverProtocol = new HashMap<>();
                serverProtocol.put(ServerProtocol.PINGPONG, new PingPong());
               // serverProtocol.put(ServerProtocol.CHAT, new Chat());

            }
            // can pass id instead of connection..
            public void handleMessage(ByteBuffer buffer, connectionHandler.ConnectionCommunicator connection) throws IOException, ClassNotFoundException {
                handlerConnectin = connection;

                Object object = deserialize(buffer);
                if (!(object instanceof ServerMessage)) {
                    throw new ClassCastException();
                }
                ServerMessage serverMessage = (ServerMessage)object;
                Protocol protocol = serverProtocol.get(serverMessage.getKey());
                protocol.action(serverMessage.getData());
            }
            private Object deserialize(ByteBuffer buffer) throws IOException, ClassNotFoundException {
                try (ObjectInputStream objOIS = new ObjectInputStream(new ByteArrayInputStream(buffer.array()))) {
                        return objOIS.readObject();
                    }
            }
            private ByteBuffer serialize(Object serverMsg) throws IOException {
                try(ByteArrayOutputStream bos = new ByteArrayOutputStream();
                    ObjectOutputStream oos = new ObjectOutputStream(bos)) {
                        oos.writeObject(serverMsg);
                        oos.flush();
                        return ByteBuffer.wrap(bos.toByteArray());
                    }
            }

            private void sendToHandler(ServerMessage servermessage) throws IOException {
                ByteBuffer message = serialize(servermessage);
                handlerConnectin.send(message);
            }



            private abstract class Protocol {
                public abstract void action(Message<?, ?> msg);
            }

            private class PingPong extends Protocol {

                @Override
                public void action(Message<?, ?> msg) {
                    if (!(msg instanceof PingPongMessage)) {
                        throw new ClassCastException();
                    }
                    System.out.println("in pingpong class " + msg.getKey());

                    PingPongKeys key = (PingPongKeys)msg.getKey();
                    PingPongMessage replyb = new PingPongMessage(key.reply());
                    ServerMessage servermessage = new ServerMessage(replyb, ServerProtocol.PINGPONG);
                    try{
                        sendToHandler(servermessage);
                    }catch (IOException e){
                        e.printStackTrace();
                    }
                }
            }

        }
    }



