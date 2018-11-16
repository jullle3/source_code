package simpel_kommunikation;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;


public class Simpel_kommunikation {

    
       static String[][] UserData = new String[][] { {"1", "Julian", "Admin"},
                {"2", "Simon", "Admin"},
                {"3", "Jakob", "Underviser"},
                {"4", "Fremmede", "Hacker"},
                {"5", "Bo", "scriptkiddy"}};

       
       
       
        // TODO: serveren skal multitrådes
    public static void main(String[] args) {
        try {
            ServerSocket serversocket = new ServerSocket(8002);
            while (true)
            {
            System.out.println("Lytter på port "+serversocket.getLocalPort());
            Socket forbindelse = serversocket.accept();
            PrintWriter ud = new PrintWriter(forbindelse.getOutputStream());
            BufferedReader ind = new BufferedReader(new InputStreamReader(forbindelse.getInputStream()));

            // Læser request
            String anmodning = ind.readLine();
            System.out.println("Server: " + anmodning);
              
            
            
            
            
            // Behandler request
            switch (anmodning.toString()){
                case "Users":
                    //ud.print(UserData);
                    ObjectOutputStream out = new ObjectOutputStream(forbindelse.getOutputStream());
                    out.writeObject(UserData);
                    break;
                    
                case "Del user":
                    System.out.println("sletter bruger...");
                    ObjectInputStream in = new ObjectInputStream(forbindelse.getInputStream());
                    String[] userInfo = (String[]) in.readObject();


                    // tjek af token
                    String token = userInfo[2];
                    if (token != "abcd") break;


                    // TODO: implementer logik til at fjerne bruger fra DB                
                    String name = userInfo[1];                    
                    
                    int ID = Integer.parseInt(userInfo[0]);
                    System.out.println("Vi fjerner brug med ID: " + ID);
                    


                default:
                    ud.println("Fejl i request");
            }
            forbindelse.close();
            }
    } catch (Exception e) {
            e.printStackTrace();
    }
    }
    
}
