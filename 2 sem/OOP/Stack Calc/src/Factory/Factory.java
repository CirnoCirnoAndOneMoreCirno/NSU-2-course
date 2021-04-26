package Factory;

import Operations.Operations;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.Properties;

public class Factory
{
    Properties props;
    public Factory()
    {
        props = new Properties();
        try
        {
            InputStream is = Factory.class.getResourceAsStream("operations.cfg");
            props.load(is);
        }
        catch (IOException e)
        {
            System.out.println(e.getMessage());
            System.exit(-1);
        }
    }
    public Operations getOperation(String command) {
        Operations op = null;
        try
        {
            String[] commandParts = command.split(" ");
            String className = props.getProperty(commandParts[0]);
            Class clazz = Class.forName(className);

            int argsCount = commandParts.length - 1;
            Class[] paramTypes = new Class[argsCount];
            Object[] params = new Object[argsCount];
            for(int i = 0; i < argsCount; i++)
            {
                paramTypes[i] = String.class;
                params[i] = commandParts[i+1];
            }

                op = (Operations) clazz.getDeclaredConstructor(paramTypes).newInstance(params);
        }
        catch (Exception e)
        {
            System.out.println(e.getMessage());
            System.exit(-1);
        }
        /*
        if (commandParts.length == 2)
        {
            op = (Operations) clazz.getDeclaredConstructor().newInstance();
        }
        */
        return op;
    }
}
