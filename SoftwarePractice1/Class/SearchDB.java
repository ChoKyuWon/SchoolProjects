import java.sql.*;

public class SearchDB {

    //Search All
    static void SearchAll(Connection connection, Statement stmt) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from user";
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" +
                    "\t\t\t\tEmail" + "\t\t\t\t\tInstrument" + "\t\t Fee");

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

    //Search by ID
    static void SearchByID(Connection connection, Statement stmt, String ID) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from user where id = " + "'" + ID + "'";
            ResultSet rs = stmt.executeQuery(sql);


            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" +
                    "\t\t\t\tEmail" + "\t\t\t\t\tInstrument" + "\t\t Fee");

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
            System.out.println("Please input right ID.");
        }
    }


    //Search by Instrumnet
    static void SearchByInst(Connection connection, Statement stmt, String INSTRUMENT) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from user where instrument = " + "'" + INSTRUMENT + "'";
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" +
                    "\t\t\t\tEmail" + "\t\t\t\t\tInstrument" + "\t\t Fee");

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
            System.out.println("Please input right Instrument.");
        }
    }


    //Search by sex
    static void SearchBySex(Connection connection, Statement stmt, String SEX) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from user where sex = " + "'" + SEX + "'";
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" +
                    "\t\t\t\tEmail" + "\t\t\t\t\tInstrument" + "\t\t Fee");

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
            System.out.println("Please input right Sex.");
        }
    }

    //Search by name
    static void SearchByName(Connection connection, Statement stmt, String NAME) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from user where name = " + "'" + NAME + "'";
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" +
                    "\t\t\t\tEmail" + "\t\t\t\t\tInstrument" + "\t\t Fee");

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
            System.out.println("Please input right Name.");
        }
    }

    //Search item by topic
    static void SearchByTopic(Connection connection, Statement stmt, String itembytopic) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from user where " + itembytopic;
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println("\tID" + "\t\t\tName" + "\t\t Sex" + "\t\tPhone" +
                    "\t\t\t\tEmail" + "\t\t\t\t\tInstrument" + "\t\t Fee");

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
            System.out.println("Please input right topic.");
        }
    }




}
