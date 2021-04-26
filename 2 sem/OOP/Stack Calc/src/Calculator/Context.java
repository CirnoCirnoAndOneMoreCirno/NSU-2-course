package Calculator;

import java.util.HashMap;
import java.util.Stack;
import Exceptions.NoSuchDefine;
import Exceptions.StackIsEmpty;

public class Context {
    Stack<Double> stack;
    HashMap<String, Double> defines;

    public Context()
    {
        stack = new Stack<Double>();
        defines = new HashMap<String, Double>();
    }
    public double getTopOfStack()
    {
        try
        {

            if (stack.isEmpty()) {
                throw new StackIsEmpty();
            }
        }
        catch (StackIsEmpty e)
        {
            System.out.println(e.getMessage());
            System.exit(-1);
        }
        return stack.pop();
    }
    public void pushToStack(double val)
    {
        stack.push(val);
    }
    public void pushToStack(String define)
    {
        try {


            if (!defines.containsKey(define)) {
                throw new NoSuchDefine();
            }
            stack.push(defines.get(define));
        }
        catch (NoSuchDefine e)
        {
            System.out.println(e.getMessage(define));
            System.exit(-1);
        }
    }
    public void pushDefine(String define, double value)
    {
        defines.put(define, value);
    }

}
