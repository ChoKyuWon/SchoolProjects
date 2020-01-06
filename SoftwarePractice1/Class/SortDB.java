import java.sql.*;

public class SortDB {

    //Sort by ID
    static void SortById(Connection connection, Statement stmt, String order) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            ResultSet rs;

            if(order.equalsIgnoreCase("asc")){
                sql = "select * from user order by id asc";
                rs = stmt.executeQuery(sql);
            }
            else if(order.equalsIgnoreCase("desc")){
                sql = "select * from user order by id desc";
                rs = stmt.executeQuery(sql);
            }
            else{
                System.out.println("Please input right order.");
                return;
            }
            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" + "\t\t\t\tEmail" +
                    "\t\t\t\t\tInstrument" + "\t\t Fee");
            while (rs.next()) {
                String id = rs.getString("ID");
                String name = rs.getString("name");
                String sex = rs.getString("sex");
                String phone = rs.getString("phone");
                String email = rs.getString("email");
                String instrument = rs.getString("instrument");
                int fee = rs.getInt("fee");

                System.out.printf("%10s\t  ", id);
                System.out.printf("%5s\t\t", name);
                System.out.printf("%3s\t\t", sex);
                System.out.printf("%15s\t\t", phone);
                System.out.printf("%-20s\t\t", email);
                System.out.printf("%-10s\t\t", instrument);
                System.out.printf("%3d\n", fee);


            }
            rs.close();
            stmt.close();


        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    //Sort by ID
    static void SortByName(Connection connection, Statement stmt, String order) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            ResultSet rs;

            if(order.equalsIgnoreCase("asc")){

                sql = "select * from user order by name asc";
                rs = stmt.executeQuery(sql);

            }
            else if(order.equalsIgnoreCase("desc")){

                sql = "select * from user order by name desc";
                rs = stmt.executeQuery(sql);
            }
            else{
                System.out.println("Please input right.");
                return;
            }


            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" + "\t\t\t\tEmail" +
                    "\t\t\t\t\tInstrument" + "\t\t Fee");

            while (rs.next()) {
                String id = rs.getString("ID");
                String name = rs.getString("name");
                String sex = rs.getString("sex");
                String phone = rs.getString("phone");
                String email = rs.getString("email");
                String instrument = rs.getString("instrument");
                int fee = rs.getInt("fee");

                System.out.printf("%10s\t  ", id);
                System.out.printf("%5s\t\t", name);
                System.out.printf("%3s\t\t", sex);
                System.out.printf("%15s\t\t", phone);
                System.out.printf("%-20s\t\t", email);
                System.out.printf("%-10s\t\t", instrument);
                System.out.printf("%3d\n", fee);


            }
            rs.close();
            stmt.close();


        } catch (Exception e) {
            e.printStackTrace();
        }
    }


}
