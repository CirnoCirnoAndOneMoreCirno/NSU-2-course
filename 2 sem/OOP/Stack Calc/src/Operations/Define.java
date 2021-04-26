package Operations;

import Calculator.Context;

public class Define extends Operations
{
    private String key;
    private double value;
    public Define(String k, String v)
    {
        key = k;
        value = Double.parseDouble(v);
    }

    @Override
    public void exec(Context cont)
    {
        cont.pushDefine(key, value);
    }
}
