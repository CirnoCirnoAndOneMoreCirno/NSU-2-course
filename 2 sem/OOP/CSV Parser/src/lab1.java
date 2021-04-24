import java.io.*;

public class lab1 {
    public static void main(String args[])
    {
        BufferedReader reader = null;
        BufferedWriter writer = null;
        try
        {
            //reader = new BufferedReader(new FileReader(args[0]));
            //writer = new BufferedWriter(new FileWriter(args[1]));
            Parser parser = new Parser(args[0], args[1], ';');
            parser.parse();
        }
        catch (Exception e) {
            System.err.println(e.getLocalizedMessage());
        }
    }
}


