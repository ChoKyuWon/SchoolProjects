import java.sql.*;

public class DatabasePractice {

    public static void main(String[] args) {
        Connection connection = null;
        Statement stmt = null;
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/bear?serverTimezone=UTC&autoReconnect=false&useSSL=false", "BearHunter", "950507");
            System.out.println("DB connection OK!");
        }
        catch (ClassNotFoundException e){
            e.printStackTrace();
            System.out.println("DB Driver Error!");
        }
        catch (SQLException se){
            se.printStackTrace();
            System.out.println("DB connection Error!");
        }

        try{
            //----Start----
            stmt = connection.createStatement();
            String sql;
            sql = "select * from bear1";
            ResultSet rs = stmt.executeQuery(sql);

            while(rs.next()){
                String name = rs.getString("name");
                String age = rs.getString("age");

                System.out.println("Name: " + name + "\tAge: " + age);
            }
            rs.close();
            stmt.close();
            connection.close();


        }
        catch (Exception e){
            e.printStackTrace();
        }

    }

}
