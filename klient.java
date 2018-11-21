import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.PrintWriter;
import java.net.Socket;



public class klient {
    public static void main(String[] args) throws ClassNotFoundException {
        
        try {
            Socket s = new Socket("localhost", 8002);
            
            PrintWriter ud = new PrintWriter(s.getOutputStream());
            BufferedReader ind = new BufferedReader(new InputStreamReader(s.getInputStream()));
	
            ud.println("Del user");
            ud.flush();
            
            //String input = ind.readLine();
            ObjectInputStream in = new ObjectInputStream(s.getInputStream());
            String userData = (String) in.readObject();
            System.out.println("Modtog: " + userData);


            
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        
    }
}
