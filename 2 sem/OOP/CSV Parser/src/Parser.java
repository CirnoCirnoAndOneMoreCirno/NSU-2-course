import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Parser {
    private int totalWordsCount;
    private final BufferedReader reader;
    private final BufferedWriter writer;
    private final HashMap<String, Integer> table;
    private final ArrayList<Map.Entry<String, Integer>> sortedTable;
    private final char delimiter;

    public Parser(String rString, String wString, char delim) throws IOException
    {
        reader = new BufferedReader(new FileReader(rString));
        writer = new BufferedWriter(new FileWriter(wString));
        table = new HashMap<>();
        sortedTable = new ArrayList<>();
        totalWordsCount = 0;
        delimiter = delim;
    }
    public void parse() throws IOException {
        readFileAndFillTable();
        sortTable();
        writeToFile();
        closeStreams();
    }
    private void readFileAndFillTable() throws IOException {
        String line;
        while ((line = reader.readLine()) != null)
        {
            String words[] = line.split("\\W");
            for(String word : words)
            {
                if (word.length() != 0)
                {
                    totalWordsCount++;
                    if(table.containsKey(word))
                    {
                        table.put(word, table.get(word) + 1);
                    }
                    else
                    {
                        table.put(word, 1);
                    }
                }
            }
        }
    }
    private void sortTable()
    {
        sortedTable.addAll(table.entrySet());
        sortedTable.sort((e1, e2) -> e2.getValue().compareTo(e1.getValue()));
    }

    private void writeToFile() throws IOException {
        for(var line : sortedTable)
        {
            writer.write(line.getKey() + delimiter + line.getValue().toString() + delimiter
                    + (((double)line.getValue() / totalWordsCount)*100) + "%\n");
        }
        writer.flush();

    }
    private void closeStreams() throws IOException
    {

        reader.close();
        writer.close();
    }
}
