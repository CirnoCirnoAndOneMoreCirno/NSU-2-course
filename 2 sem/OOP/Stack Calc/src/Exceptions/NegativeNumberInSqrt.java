package Exceptions;

public class NegativeNumberInSqrt extends Exception{
    @Override
    public String getMessage() {
        return "Negative number in square root";
    }
}
