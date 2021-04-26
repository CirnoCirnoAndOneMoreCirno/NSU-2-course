package Exceptions;

public class DivisionByZero extends Exception{
    @Override
    public String getMessage()
    {
        return "Division by zero";
    }
}
