package Calculator;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public abstract class InputReader
{
    public abstract ArrayList<String> getCommands() throws IOException;
}

class ConsoleReader extends InputReader
{
    Scanner scan;
    public ConsoleReader()
    {
        scan = new Scanner(System.in).useDelimiter("\n");
    }

    @Override
    public ArrayList<String> getCommands() throws IOException
    {
        String line;
        ArrayList<String> commands = new ArrayList<String>();
        while((line = scan.nextLine()).length() > 0)
        {
            commands.add(line);
        }
        return commands;
    }
}

class FileInputReader extends InputReader
{
    private BufferedReader reader;
    public FileInputReader(BufferedReader r)
    {
        reader = r;
    }

    @Override
    public ArrayList<String>  getCommands() throws IOException
    {
        String line;
        ArrayList<String> commands = new ArrayList<String>();
        while((line = reader.readLine()) != null)
        {
            commands.add(line);
        }
        return commands;
    }
}