import java.sql.*;

public class ModifyDB {

    //Case1 : change value except fee
    static void Modify(Connection connection, Statement stmt, String changefield, String changevalue, String changewho) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "update user set " + changefield + " = '" + changevalue + "' where id = '" + changewho + "'";
            int rs = stmt.executeUpdate(sql);

            System.out.printf("%s's information is Modified.\n", changewho);


            stmt.close();


        } catch (Exception e) {
            System.out.println("Can't modify the value!");
            System.out.println("Please write ID exactly.");
        }
    }

    //Case2 : change fee
    static void Modify(Connection connection, Statement stmt, boolean changevalue, String changewho) {
        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "update user set fee =" + changevalue + " where id ='" + changewho + "'";
            int rs = stmt.executeUpdate(sql);

            System.out.printf("%s's information is Modified.\n", changewho);

            stmt.close();


        } catch (Exception e) {
            System.out.println("Can't modify the value!");
            System.out.println("Please write ID exactly.");
        }
    }
}
