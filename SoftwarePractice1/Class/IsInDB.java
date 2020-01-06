import java.sql.*;
public class IsInDB {

    static int isindb(Connection connection, Statement stmt, String ID) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from user where id = " + "'" + ID + "'";
            ResultSet rs = stmt.executeQuery(sql);

            //No data
            if(!rs.next())
                return 0;
            else {
                SearchDB.SearchByID(connection, stmt, ID);
            }

            rs.close();
            stmt.close();

            return 1;

        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }

    }
}
