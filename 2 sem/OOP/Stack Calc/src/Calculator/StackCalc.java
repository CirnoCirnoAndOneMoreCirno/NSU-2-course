package Calculator;

import Factory.Factory;
import Operations.Operations;

import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;

public class StackCalc
{
    ArrayList<String> commands;
    InputReader iReader;
    Context context;
    public StackCalc(BufferedReader r)
    {
        iReader = new FileInputReader(r);
        context = new Context();
    }
    public StackCalc()
    {
        iReader = new ConsoleReader();
        context = new Context();
    }
    public  void calculate()
    {
        try
        {
            commands = iReader.getCommands();
        }
        catch (Exception e)
        {
            System.out.println(e.getMessage());
        }
        if (commands.isEmpty())
        {
                //Exception here
                //or in InputReader
                //or do nothing
        }

        Factory fact = new Factory();
        for (var line : commands)
        {
            Operations op = fact.getOperation(line);
            op.exec(context);
        }
    }

}
