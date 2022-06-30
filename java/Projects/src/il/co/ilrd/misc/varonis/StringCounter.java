package il.co.ilrd.misc.varonis;

import java.io.*;

import java.util.*;

//Varonis
public class StringCounter  {
    //private LinkedList<Integer> list = new LinkedList<>();
    private HashMap<String,Oper> operMap = new HashMap<>();
    private HashMap<String, Integer> dictionary;
    private static String path;
    public StringCounter(String files) {
        dictionary = new HashMap<>();
        path = files;

        operMap.put("sort -d",sortDesc);
        operMap.put("sort -a",this::sortAsc);
        operMap.put("count", this::produceOutput);
        operMap.put("by", this::maxWord);

    }
    public void operate(String str) throws IOException {
        operMap.get(str).oper();
    }


    interface Oper{
         void oper() throws IOException;
    }
    public void counter(){
        List<String> textFileList = Arrays.asList(path);
        try {
            Dictionary reader = new Dictionary(dictionary);

            for (String text : textFileList) {
                reader.fileScanner(text);
            }

            Scanner textFile = new Scanner(new File(path));
            ArrayList<String> file = new ArrayList<>();

            while (textFile.hasNext()) {
                file.add(textFile.next().trim().toLowerCase());
            }

            for (String word : file) {
                dictionary.merge(word, 1, Integer::sum);
            }
            textFile.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    public void produceOutput() throws IOException {

        FileWriter writing = new FileWriter("/Users/tanyashkolnik/Documents/F2.txt", false);

        try (BufferedWriter writer = new BufferedWriter(writing)) {
            for (Map.Entry<String, Integer> entry : dictionary.entrySet()) {
                writer.write(entry.getKey() + " : " + entry.getValue());
                writer.newLine();
            }
        }
        System.out.println("created file: F2.txt with words and their according count");
    }
    public void maxWord(){
        Integer max = Dictionary.getMax();
        for (Map.Entry<String, Integer> entry : dictionary.entrySet()) {
            if(entry.getValue().equals(max)) {
                System.out.println("most common word is");
                System.out.println(entry.getKey() + ": " + entry.getValue());
            }
        }
    }

    public void printSplit(String some) throws IOException {
        FileWriter writing = new FileWriter("/Users/tanyashkolnik/Documents/F2.txt", false);
        List<String> sorted = new ArrayList<>(dictionary.keySet());

        Collections.sort(sorted);
        if(some.equals("-c"))
            some = ",";
        if(some.equals("-s"))
            some = " ";
        if(some.equals("-n"))
            some = "  ";

        try (BufferedWriter writer = new BufferedWriter(writing)) {
            for (String w : sorted) {
                writer.write(w);
                writer.write(some);
                if(some.equals("  "))
                     writer.newLine();
            }

        }
        System.out.println("created file: F2.txt with the desired split");
    }
    Oper sortDesc = () -> {
        FileWriter writing = new FileWriter("/Users/tanyashkolnik/Documents/F2.txt", false);
        List<String> sorted = new ArrayList<>(dictionary.keySet());

        Collections.sort(sorted);
        Collections.reverse(sorted);
        try (BufferedWriter writer = new BufferedWriter(writing)) {
            for (String w : sorted) {
                writer.write(w);
                writer.newLine();
            }

        }
        System.out.println("created file: F2.txt with the desired sort");
    };

    public  void sortAsc() throws IOException {
        FileWriter writing = new FileWriter("/Users/tanyashkolnik/Documents/F2.txt", false);
        List<String> sorted = new ArrayList<>(dictionary.keySet());
        Collections.sort(sorted);
        try (BufferedWriter writer = new BufferedWriter(writing)) {
            for (String w : sorted) {
                writer.write(w);
                writer.newLine();
            }

        }
        System.out.println("created file: F2.txt with the desired sort");
    }

}


