package il.co.ilrd.networking;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import static il.co.ilrd.networking.ClientUDP.echo;


public class ServerUDP {
    private DatagramSocket udpSocket;
    private int port;
    private volatile boolean isRun;
    private DatagramPacket incoming;

    public ServerUDP(int port){
        this.port = port;
        isRun = true;
    }

    private void listen() throws Exception{

        echo("Server is listening on port: "+ port+ "" + InetAddress.getLocalHost() + "--");
        this.udpSocket = new DatagramSocket(port);
        byte[] buf = new byte[1024];
        incoming = new DatagramPacket(buf, buf.length);

        String msg;
        String sendMsg = "ping";

        while(isRun){
            udpSocket.receive(incoming);

            msg = new String(incoming.getData(),0, incoming.getLength());

            echo(msg);

            DatagramPacket dp = new DatagramPacket(sendMsg.getBytes() , sendMsg.getBytes().length , incoming.getAddress() , incoming.getPort());
            udpSocket.send(dp);
        }
    }

    public static void main(String[] args) throws Exception {
        ServerUDP udp = new ServerUDP(7878);
        udp.listen();
    }
}
