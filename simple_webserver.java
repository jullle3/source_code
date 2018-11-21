import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;


    public class Server {


        static String[][] UserData = new String[][] { {"1", "Julian", "Admin"},
                    {"2", "Simon", "Admin"},
                    {"3", "Jakob", "Underviser"},
                    {"4", "Fremmede", "Hacker"},
                    {"5", "Bo", "scriptkiddy"}};




            // TODO: serveren skal multitrådes
        public static void main(String[] args) 
        {
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
                System.out.println("Behandler request \"" + anmodning + "\"");
                ObjectOutputStream out = new ObjectOutputStream(forbindelse.getOutputStream());



                
                // Behandler request
                switch (anmodning.toString()){
                    case "Users":
                        getUsers(out);
                        break;

                    case "Del user":
                        deleteUser(forbindelse);
                        break;
                    case "System status":
                        getSystemStatus(out);
                        break;

                    default:
                        ud.println("Fejl i request");
                }
                forbindelse.close();
                }
            } catch (IOException | ClassNotFoundException e) {
                    e.printStackTrace();
            }
        }

        
        private static void getSystemStatus(ObjectOutputStream out) throws MalformedURLException, ProtocolException, IOException
        {
            // get https://api.thingspeak.com/channels/620356/fields/1.json?results=1
            // HTTP GET request

            URL obj = new URL("https://api.thingspeak.com/channels/620356/fields/1.json?results=1");
            HttpURLConnection con = (HttpURLConnection) obj.openConnection();

            // optional default is GET
            con.setRequestMethod("GET");


            int responseCode = con.getResponseCode();
            System.out.println("\nSending 'GET' request to thingspeak.com : ");
            System.out.println("Response Code : " + responseCode);

            BufferedReader in_ny = new BufferedReader(new InputStreamReader(con.getInputStream()));
            String inputLine;

            //StringBuffer response = new StringBuffer();

            String response = "Empty"; 

            while ((inputLine = in_ny.readLine()) != null) {
                    response += inputLine ;
            }
            in_ny.close();

            //print result
            System.out.println(response);
            out.writeObject(response);
            out.flush();
            //ud.println(response.toString());
            //ud.flush();

        }
        
        private static void getUsers(ObjectOutputStream out) throws MalformedURLException, ProtocolException, IOException{
                out.writeObject(UserData);

        }
        
        private static void deleteUser(Socket forbindelse) throws MalformedURLException, ProtocolException, IOException, ClassNotFoundException{
            System.out.println("Venter på input fra klient...");
            ObjectInputStream in = new ObjectInputStream(forbindelse.getInputStream());
            String[] userInfo = (String[]) in.readObject();


            // tjek af token
            String token = userInfo[2];
            if (token != "abcd") {
            System.out.println("Wrong token");
            return;
            }


            // TODO: implementer logik til at fjerne bruger fra DB                
            String name = userInfo[1];                    

            int ID = Integer.parseInt(userInfo[0]);
            System.out.println("sletter brug med ID: " + ID);

        }
        
    }
