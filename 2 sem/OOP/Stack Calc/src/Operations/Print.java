package Operations;

import Calculator.Context;

public class Print extends Operations{
    @Override
    public void exec(Context cont) {
        double op = cont.getTopOfStack();
        System.out.println(op);
        //System.out.flush();
        cont.pushToStack(op);
    }
}
