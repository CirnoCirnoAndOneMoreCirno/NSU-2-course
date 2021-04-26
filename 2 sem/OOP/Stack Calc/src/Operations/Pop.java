package Operations;

import Calculator.Context;

public class Pop extends Operations{
    @Override
    public void exec(Context cont) {
        cont.getTopOfStack();
    }
}
