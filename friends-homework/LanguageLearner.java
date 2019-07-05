import java.io.*;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Collections;

public class LanguageLearner {

    // take a line, and break it up into a list of words
    private ArrayList<String> parseWords(String line) {
        ArrayList<String> wordList = new ArrayList<String>();
        
        // split the line by spaces, then loop through each word
        for (String s : line.split(" ")) {
            if (s != null && !s.trim().isEmpty()) {
                // if the word is not null or emptystring, add it to the word list
                wordList.add(s);
            }
        }
        return wordList;
    }

    // takes filepath, returns array of lines
    private ArrayList<String> readFile(String filepath) {
        BufferedReader in = null;
        ArrayList<String> lineList = new ArrayList<String>();
        // try opening the file at filepath
        try {   
            in = new BufferedReader(new FileReader(filepath));
            String str;
            // while there are more lines
            while ((str = in.readLine()) != null) {
                // add each line to the list of lines
                lineList.add(str);
            }
            if (in != null) {
                in.close(); // close the file when we're finished with it
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found!");
        } catch (IOException e) {
            System.out.println("Problem opening file!");
            // print the exact error that occured
            e.printStackTrace();
        }
        return lineList;
    }

    // takes directoryPath, returns array of allWords in whole directory
    private ArrayList<String> readDirectory(String directoryPath, int nFiles) {
        ArrayList<String> allWords = new ArrayList<String>();
        // loop nFiles times
        for (int i=1; i<=nFiles; i++) {
            // use int n to create the filepath e.g. "directoryname/1.txt"
            String filePath = directoryPath + "/" + Integer.toString(i) + ".txt";

            // for each line in the file
            for (String line : readFile(filePath)) {
                // parse out individual words, then add them all to the allWords list
                allWords.addAll(parseWords(line));
            }
        }

        return allWords;
    }
   
    // return a word frequency table build from nFiles in specified directoryPath
    public HashMap<String, Integer> countWords(String directoryPath, int nFiles) {
        HashMap<String, Integer> wordTable = new HashMap<String, Integer>();

        // for each word in the list of all words in the directory
        for (String word : readDirectory(directoryPath, nFiles)) {
            
            // check if it's already in our table
            Integer count = wordTable.get(word);

            if (count == null) {
                // if not, add it with 1 to start
                wordTable.put(word, 1);
            } else {
                // if we already have it, increment the frequency by 1
                wordTable.put(word, count+1);
            }
        }
        return wordTable;
    }

    // write a frequency hashmap to a file
    public void writeVocabulary(HashMap<String, Integer> wordMap, String filepath) {
        ArrayList<String> sortedKeys = new ArrayList<String>();
        // turn the hashmaps keys into an array, and add them to the sortedKeys list
        Collections.addAll(sortedKeys, wordMap.keySet().toArray(new String[0]));
        
        // sort the list in alphabetical order
        Collections.sort(sortedKeys, String.CASE_INSENSITIVE_ORDER);

        // try creating a file and writing to it
        try {
            PrintWriter writer = new PrintWriter(filepath, "UTF-8");
            // for each word in the hashmap
            for (String key : sortedKeys) {
                // write "word 123\n" where 123 is the frequency
                writer.println(key + " " + Integer.toString(wordMap.get(key)));
            }
            writer.close();   
        } catch (FileNotFoundException e) {
            System.out.println("Invalid output path!");
        } catch (UnsupportedEncodingException e) {
            System.out.println("Your system is insane, it doesn't support UTF-8.");
        }
    }

    // print out a frequency hashmap for a human to read
    public void printWordTable(HashMap<String, Integer> wordMap) {
        for (String key : wordMap.keySet()) {
            // for each key in the hashmap, print word space frequency
            System.out.printf("%s %d\n", key, wordMap.get(key));
        }
    }

    public static void main(String[] args) {
        LanguageLearner langLearner = new LanguageLearner();
        
        // generate two word frequency hashmaps, one from 20 files in docs/train/eng and one from fre
        System.out.println("Reading wordsets:");
        HashMap<String, Integer> english = langLearner.countWords("docs/train/eng", 20);
        System.out.println("  docs/train/eng - size: " + Integer.toString(english.size()));
        HashMap<String, Integer> french = langLearner.countWords("docs/train/fre", 20);
        System.out.println("  docs/train/fre - size: " + Integer.toString(french.size()));

        // print out the hashmaps for testing
        // System.out.println("English:");
        // langLearner.printWordTable(english);
        // System.out.println();
        // System.out.println("French:");
        // langLearner.printWordTable(french);

        // write both hashmaps to files
        System.out.println("Wrote two word => frequency files:");
        langLearner.writeVocabulary(english, "eng_vocab.txt");
        System.out.println("  eng_vocab.txt");
        langLearner.writeVocabulary(french, "fre_vocab.txt");
        System.out.println("  fre_vocab.txt");
    }
}
