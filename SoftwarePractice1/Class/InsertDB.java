import java.sql.*;

public class InsertDB {


    //static method
    static void Insert(Connection connection, Statement stmt,
                       String ID, String NAME, String SEX, String PHONE, String EMAIL, String INSTRUMENT, boolean FEE) {

        try {
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "insert into user values" + "('" + ID + "'," + "'" + NAME + "'," + "'" + SEX +
                    "'," + "'" + PHONE + "'," + "'" + EMAIL + "'," + "'" + INSTRUMENT + "'," + FEE + ")";
            int rs = stmt.executeUpdate(sql);

            System.out.println(ID + "'s information is inserted.");

            stmt.close();



        } catch (Exception e) {
            System.out.println("It is already exist, or some value is too big!!");
        }

    }
}
