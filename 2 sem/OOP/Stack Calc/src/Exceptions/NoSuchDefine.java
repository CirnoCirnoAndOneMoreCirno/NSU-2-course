package Exceptions;

public class NoSuchDefine extends Exception{
    public String getMessage(String def) {
        return ("No such define " + def);
    }
}
