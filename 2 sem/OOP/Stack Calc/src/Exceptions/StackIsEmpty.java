package Exceptions;

public class StackIsEmpty extends Exception
{
    @Override
    public String getMessage() {
        return "Stack is empty; Can't pop.";
    }
}
