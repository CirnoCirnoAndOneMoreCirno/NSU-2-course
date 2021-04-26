package Operations;

import Calculator.Context;
import Exceptions.NegativeNumberInSqrt;

public class Sqrt extends Operations{
    @Override
    public void exec(Context cont)
    {
        try {
            double op = cont.getTopOfStack();
            if (op < 0) {
                throw new NegativeNumberInSqrt();
            }
            cont.pushToStack(Math.sqrt(op));
        }
        catch (NegativeNumberInSqrt e)
        {
            System.out.println(e.getMessage());
            System.exit(-1);
        }
    }
}
