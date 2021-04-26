package Operations;

import Calculator.Context;
import Exceptions.DivisionByZero;

public class Divide extends Operations
{
    @Override
    public void exec(Context cont) {
        try
        {
            double p1 = cont.getTopOfStack();
            double p2 = cont.getTopOfStack();
            if (p2 < 0.0000001) {
                throw new DivisionByZero();
            }
            cont.pushToStack(p1 / p2);
        }
        catch (DivisionByZero e)
        {
            System.out.println(e.getMessage());
            System.exit(-1);
        }
    }
}
