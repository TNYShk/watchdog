package tcode.wordle;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashSet;

/*
    english dictionary loaded into hashSet,
    each input string permutates
    only available words (in the dictionary) are printed.
    WORDLE here we go:)
 */
public class Wordle {

    private static char[] charArray;
    private static HashSet<String> dictionary = new HashSet<>();
    //public static Scanner sc = new Scanner(System.in);
    //static String wor = sc.nextLine();

/*    public static void loadDictionary(String word){
        while(!wor.equals("666")){
            word = wor;
            if(dictionary.contains(word))
                System.out.println(word);
            wor = sc.nextLine();
        }
    }*/
    public static boolean findInDictionary(String word) throws IOException {
        try( BufferedReader rd = new BufferedReader( new FileReader ("/Users/tanyashkolnik/Documents/Education/Infinity/words.txt"))) {
            String line;
            int i = 0;
            while ((line = rd.readLine()) != null) {
                dictionary.add(line);
                i++;
            }
        }
        return dictionary.contains(word);
    }
    public static void printPermutation(String str) throws IOException {
            Permutations("", str);
        }

    private static void Permutations(String prefix, String str) throws IOException {
        if(0 == str.length()){
           // if(findInDictionary(prefix)) /* local file*/
            if(isInMiriamWebsterDict(prefix)) /* MiriamWebster thesaurus API*/
                //if(findOnline(prefix)) /* free dictionary API*/
                    System.out.print(prefix + " ");
            return;
        }

        int length = str.length();
        for (int i = 0; i < length; ++i) {
            Permutations(prefix + str.charAt(i), str.substring(0, i) + str.substring(i + 1, length));
        }
    }

    private static void printAllPermutation(String str) {
        int start = 0;
        int end = str.length() - 1;
        charArray = str.toCharArray();
        printAllPermutationRec( start, end);
    }

    private static void printAllPermutationRec( int start, int end){
        if (start == end) {
                System.out.println(charArray);
            return;
        }
        for (int i = start; i <= end; ++i) {
            charArray = swap( start, i);
            printAllPermutationRec( start + 1, end);
            charArray = swap( start, i);
        }
    }

    private static char[] swap( int start, int end) {
        char tmp = charArray[start];
        charArray[start] = charArray[end];
        charArray[end] = tmp;

        return charArray;
    }


    public static boolean findOnline(String w){
   
        boolean isWord;
        String requestURL = "https://api.dictionaryapi.dev/api/v2/entries/en/";
        requestURL = requestURL.concat(w);
       // System.out.println(requestURL);
        try {
            HttpUtility.sendGetRequest(requestURL);
            String result = HttpUtility.readSingleLineRespone();
           /* String[] response = Dictionary.readMultipleLinesRespone();
            for (String line : response) {
                System.out.println(line);
            }*/
            //System.out.println(result);
            isWord =  true;
        } catch (IOException ex) {
            isWord =  false;

        }finally {
            HttpUtility.disconnect();
        }
        return isWord;
    }

    public static boolean isInMiriamWebsterDict(String word) {

        boolean isFound = false;
        String requestURL = "https://dictionaryapi.com/api/v3/references/ithesaurus/json/";
        String myAPI = "?key=e3b0322b-c7f8-401c-ac77-ca4c84aa62a5";
        requestURL = requestURL.concat(word);
        requestURL = requestURL.concat(myAPI);

        try {
            HttpUtility.sendGetRequest(requestURL);
            String result = HttpUtility.readSingleLineRespone();
            dictionary.add(result);
            //String[] response = Dictionary.readMultipleLinesRespone();
            System.out.println(result);
            if (!result.isEmpty()){
                isFound = true;
            }
        } catch (IOException e) {
            return false;
        }finally {
            HttpUtility.disconnect();
        }
        return isFound;
    }

        public static void main(String[] args) throws IOException {
        String str = "tunip";
        printPermutation(str);

        //String str1 = "fault";

        //printAllPermutation(str);
       // printPermutation("fault");
        //printAllPermutation(str1);

    }
}