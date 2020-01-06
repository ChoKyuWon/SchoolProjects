import java.sql.*;

public class GroupByInst {

    //Search All
    static void groupByInst(Connection connection, Statement stmt) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select count(*) as number, instrument from user group by instrument";
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println(" Number" + "\t\tInstrument");

            while (rs.next()) {
                String number = rs.getString("number");
                String instrument = rs.getString("instrument");

                System.out.printf("%4s\t\t ", number);
                System.out.printf("%5s\t\t\n", instrument);


            }
            rs.close();
            stmt.close();


        } catch (Exception e) {
            e.printStackTrace();
        }
    }



}
