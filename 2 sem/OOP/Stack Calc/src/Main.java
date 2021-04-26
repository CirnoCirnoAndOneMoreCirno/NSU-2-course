import Calculator.StackCalc;

import java.io.*;

public class Main
{
    public static void main(String[] args) throws Exception
    {
        BufferedReader r = new BufferedReader(new FileReader("Input"));

        //StackCalc calc = new StackCalc(r);
        StackCalc calc = new StackCalc(r);

        calc.calculate();
    }
}
