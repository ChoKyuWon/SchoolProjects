import java.sql.*;
import java.util.Scanner;

public class BandClub {

    public static void main(String[] args) {

        Connection connection = null;
        Statement stmt = null;
        Scanner scanner = new Scanner(System.in);
        String id, name, sex, phone, email, instrument, changefield, changevalue, type, order, itembytopic;
        boolean fee;
        boolean wantexit = true;
        int isin;

        //Check Connection
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/BandClub?serverTimezone=UTC&" +
                    "autoReconnect=false&useSSL=false", "BearHunter", "950507");
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


        while(wantexit){

            System.out.println("1. Insert");
            System.out.println("2. Modify");
            System.out.println("3. Delete");
            System.out.println("4. Search");
            System.out.println("5. Sort");
            System.out.println("6. Show number of Instrument, individually");
            System.out.println("7. Exit");


            int choose = scanner.nextInt();

            switch (choose){

                //Insert
                case 1:
                    System.out.println("Please input (ID, NAME, SEX, PHONE, EMAIL, INSTRUMENT, FEE)");
                    System.out.printf("ID (10 number) : ");
                    id = scanner.next();
                    System.out.println();
                    System.out.printf("NAME : ");
                    name = scanner.next();
                    System.out.println();
                    System.out.printf("SEX (M, F) : ");
                    sex = scanner.next();
                    System.out.println();
                    System.out.printf("PHONE (include '-') : ");
                    phone = scanner.next();
                    System.out.println();
                    System.out.printf("EMAIL : ");
                    email = scanner.next();
                    System.out.println();
                    System.out.printf("INSTRUMENT : ");
                    instrument = scanner.next();
                    System.out.println();
                    System.out.printf("FEE (true or false) : ");
                    fee = scanner.nextBoolean();
                    System.out.println();

                    InsertDB.Insert(connection, stmt, id, name, sex, phone, email, instrument, fee);

                    break;

                //Modify
                case 2:
                    System.out.println("Who's data?");
                    System.out.print("ID : ");
                    id = scanner.next();

                    isin = IsInDB.isindb(connection, stmt, id);

                    if(isin == 0){
                        System.out.println("There's no data.");
                        break;
                    }

                    System.out.println("What do you want to change?");
                    System.out.print("(Phone or Email or Instrument or fee) : ");
                    changefield = scanner.next();

                    if(changefield.equalsIgnoreCase("name")
                             || changefield.equalsIgnoreCase("id")
                             || changefield.equalsIgnoreCase("sex")){
                        System.out.println("Can't modify that field!!");
                        break;
                    }


                    else if(changefield.equalsIgnoreCase("fee")){
                        System.out.println("What value?(true or false) : ");
                        fee = scanner.nextBoolean();
                        System.out.println("Are you sure change this data?");
                        System.out.print("Yes or No? : ");
                        type = scanner.next();
                        if(type.equalsIgnoreCase("yes"))
                            ModifyDB.Modify(connection, stmt, fee, id);
                    }


                    else {
                        System.out.println("What value? : ");
                        changevalue = scanner.next();
                        System.out.println("Are you sure change this data?");
                        System.out.print("Yes or No? : ");
                        type = scanner.next();

                        if(type.equalsIgnoreCase("yes"))
                            ModifyDB.Modify(connection, stmt, changefield, changevalue, id);
                    }

                    break;

                //Delete
                case 3:
                    System.out.println("Who's data?");
                    System.out.print("ID : ");
                    id = scanner.next();

                    isin = IsInDB.isindb(connection, stmt, id);

                    if(isin == 0){
                        System.out.println("There's no data.");
                        break;
                    }
                    else {

                        System.out.println("Are you sure delete this data?");
                        System.out.print("Yes or No? : ");
                        changevalue = scanner.next();

                        if(changevalue.equalsIgnoreCase("yes")){
                            DeleteDB.Delete(connection, stmt, id);
                        }
                    }

                    break;

                //Search
                case 4:

                    System.out.println("Type? (All, ID, Instrument, Name, Sex, Item by Topic)");
                    System.out.print("Type : ");
                    scanner.nextLine();
                    type = scanner.nextLine();


                    if(type.equalsIgnoreCase("all"))
                        SearchDB.SearchAll(connection, stmt);

                    else if(type.equalsIgnoreCase("id")) {
                        System.out.print("ID : ");
                        id = scanner.next();
                        SearchDB.SearchByID(connection, stmt, id);
                    }
                    else if(type.equalsIgnoreCase("item by topic")){
                        System.out.println("Input : ");
                        itembytopic = scanner.nextLine();
                        SearchDB.SearchByTopic(connection, stmt, itembytopic);
                    }

                    else if(type.equalsIgnoreCase("instrument")){
                        System.out.print("Instrument : ");
                        instrument = scanner.next();
                        SearchDB.SearchByInst(connection, stmt, instrument);
                    }
                    else if(type.equalsIgnoreCase("name")){
                        System.out.print("Name : ");
                        name = scanner.next();
                        SearchDB.SearchByName(connection, stmt, name);
                    }
                    else if(type.equalsIgnoreCase("sex")){
                        System.out.print("Sex (M or F) : ");
                        sex = scanner.next();
                        SearchDB.SearchBySex(connection, stmt, sex);
                    }

                    else{
                        System.out.println("Please input right Type.");
                    }


                    break;

                //Sort
                case 5:
                    System.out.println("What would you like to sort by?");
                    System.out.print("By (ID or Name) : ");
                    type = scanner.next();
                    System.out.print("Asc? Desc? : ");
                    order = scanner.next();

                    if(type.equalsIgnoreCase("id"))
                        SortDB.SortById(connection, stmt, order);
                    else if(type.equalsIgnoreCase("name"))
                        SortDB.SortByName(connection, stmt, order);


                    break;

                //number of Instrument
                case 6:
                    GroupByInst.groupByInst(connection, stmt);
                    break;

                //Exit
                case 7:
                    wantexit = false;
                    System.out.println("Bye!!");
                    break;

                    default:
                        System.out.println("Please input right number!!");
                        break;

            }


        }


    }
}
