package Operations;

import Calculator.Context;

public class Add extends Operations
{

    public void exec(Context cont)
    {
        double p1 = cont.getTopOfStack();
        double p2 = cont.getTopOfStack();
        cont.pushToStack(p1 + p2);
    }
}
