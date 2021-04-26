package Operations;

import Calculator.Context;

public class Push extends Operations{
    String strVal;
    double val;
    public Push(String s)
    {
        strVal = s;
    }

    @Override
    public void exec(Context cont) {
        try
        {
            double val = Double.parseDouble(strVal);
            cont.pushToStack(val);
        }
        catch(NumberFormatException e)
        {
            cont.pushToStack(strVal);
        }
    }
}
