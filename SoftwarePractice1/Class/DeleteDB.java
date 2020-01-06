import java.sql.*;
public class DeleteDB {

    //static method
    static void Delete(Connection connection, Statement stmt, String ID) {

        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "delete from user where id = '" + ID + "'";
            int rs = stmt.executeUpdate(sql);

            System.out.println(ID + "'s information is deleted.");

            stmt.close();



        } catch (Exception e) {
            e.printStackTrace();
        }

    }

}
