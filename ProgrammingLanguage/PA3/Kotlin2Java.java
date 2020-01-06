import java.io.*;
import java.util.*;
import java.lang.Integer;
import java.lang.Double;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

class EvalListener extends KotlinBaseListener{
    private File file;
    private FileWriter fw;
    public int debug = 0;
    EvalListener(File f){
        try{
            file = f;
            fw = new FileWriter(file, true) ;
        }catch(Exception e){
            e.printStackTrace();
        }
    }
    private void Write(Object x){
        try{
            if(debug == 1){
                System.out.print(x.toString());
            }
            else
                fw.write(x.toString());
        }catch(Exception e){
            e.printStackTrace();
        }
    }
    
    private String typeconvert(KotlinParser.TypeContext ctx){
        if(ctx == null)
            return "void";
        else{
            if(ctx.ANY() != null)
                return "Object";
            else if(ctx.INT() != null)
                return "int";
            else if(ctx.DOUBLE() != null)
                return "Double";
        }
        return "void";
    }
    
    @Override
    public void exitProg(KotlinParser.ProgContext ctx) {
        if(debug ==1){
            System.out.println();
        }
        try{
            fw.close();
        }catch(Exception e){
            e.printStackTrace();
        }
    }
    
    // @Override
    // public void enterPack(KotlinParser.PackContext ctx) {
    //     Write(ctx.getChild(0).getText() + " " + ctx.getChild(1).getText()+';\n');
    // }
    
    // @Override 
    // public void enterImport_(KotlinParser.Import_Context ctx) {
    //         Write(ctx.getChild(0).getText() + " " + ctx.getChild(1).getText()+'\n');
    // }
    @Override public void enterObjlist(KotlinParser.ObjlistContext ctx) {
        Write("public class Result{\n");
    }
    @Override public void exitObjlist(KotlinParser.ObjlistContext ctx) {
        Write("}");
    }
    @Override public void enterFunc(KotlinParser.FuncContext ctx) {
        if(ctx.modifier() != null){
            if(ctx.modifier().PRIVATE() != null){ Write(ctx.modifier().PRIVATE()+" ");}
            else{Write(ctx.modifier().PUBLIC()+" ");}
        }
        else{
            if(ctx.functionidentifier().identifier().ID().toString().equals("main")){
                Write("public ");
            }
        }
        Write("static ");
        if(ctx.type() != null)
            Write(typeconvert(ctx.type())+" ");
        else if(ctx.parameters().parameter(0)!=null)
            Write(typeconvert(ctx.parameters().parameter(0).type())+" ");
        else
            Write("void ");
        Write(ctx.functionidentifier().identifier().ID());

        Write("(");
        if(ctx.functionidentifier().identifier().ID().toString().equals("main"))
            Write("String[] args");
        else{
            if(ctx.parameters() != null){
                for(int i=0;;i++){
                    if(ctx.parameters().parameter(i)==null){
                        break;
                    }
                    if(i != 0)
                        Write(",");
                    Write(typeconvert(ctx.parameters().parameter(i).type())+" "+ctx.parameters().parameter(i).identifier().ID());
                }
            }
        }
        Write(")");
        if(ctx.innercode() != null){
            Write("{\nreturn "+ ctx.innercode().expr().getText()+";\n}");
        }
    }

    @Override public void exitFunc(KotlinParser.FuncContext ctx) {
        Write("\n");
    }
    @Override
    public void enterCode(KotlinParser.CodeContext ctx) {
        Write("{\n");
    }
    
    @Override public void exitCode(KotlinParser.CodeContext ctx) {
        Write("}");
    }
    @Override public void enterAssign(KotlinParser.AssignContext ctx) {
        Write(ctx.getText()+";\n");
    }
	@Override public void enterInnercode(KotlinParser.InnercodeContext ctx) { }
	
	@Override public void exitInnercode(KotlinParser.InnercodeContext ctx) { }
	
	@Override public void enterExpr(KotlinParser.ExprContext ctx) { }
	
	@Override public void exitExpr(KotlinParser.ExprContext ctx) { }
	
	@Override public void enterFcall(KotlinParser.FcallContext ctx) {
        if(ctx.functionidentifier().identifier().ID().toString().contains("print")){
            Write("System.out."+ctx.functionidentifier().getText()+"(");
        }
        else{
            Write(ctx.functionidentifier().getText());
            Write("(");
        }
        for(int i=0;;i++){
            if(ctx.fparam(i) == null)
                break;
            Write(ctx.fparam(i).getText());
            if(ctx.fparam(i+1) == null)
                break;
            Write(",");
        }
        Write(");\n");
    }
	
	@Override public void exitFcall(KotlinParser.FcallContext ctx) { }
	
	@Override public void enterFparam(KotlinParser.FparamContext ctx) { }
	
	@Override public void exitFparam(KotlinParser.FparamContext ctx) { }
	
	@Override public void enterS_expr(KotlinParser.S_exprContext ctx) { }
	
	@Override public void exitS_expr(KotlinParser.S_exprContext ctx) { }
	
	@Override public void enterString(KotlinParser.StringContext ctx) { }
	
	@Override public void exitString(KotlinParser.StringContext ctx) { }
	
	@Override public void enterStringContent(KotlinParser.StringContentContext ctx) { }
	
	@Override public void exitStringContent(KotlinParser.StringContentContext ctx) { }
	
	@Override public void enterStringExpression(KotlinParser.StringExpressionContext ctx) { }
	
	@Override public void exitStringExpression(KotlinParser.StringExpressionContext ctx) { }
	
	@Override public void enterStrText(KotlinParser.StrTextContext ctx) { }
	
	@Override public void exitStrText(KotlinParser.StrTextContext ctx) { }
	
	@Override public void enterStrRef(KotlinParser.StrRefContext ctx) { }
	
	@Override public void exitStrRef(KotlinParser.StrRefContext ctx) { }
	
	@Override public void enterIf_(KotlinParser.If_Context ctx) {
        Write(ctx.IF());
        Write(ctx.condition().getText());
    }
	
	@Override public void enterIfelse(KotlinParser.IfelseContext ctx) {
        for (int i = 0; ; i++){
            if(ctx.ELSE(i) == null)
                break;
            else
                Write("else if "+ctx.condition(i).getText());
        }
    }

	@Override public void enterElse_(KotlinParser.Else_Context ctx) {
        Write("else");
    }
	
	@Override public void enterFor_(KotlinParser.For_Context ctx) {
        Write("for(");
    }
	
	@Override public void enterForcondition(KotlinParser.ForconditionContext ctx) {
        if(ctx.modifier() != null)
            Write(ctx.modifier()+" ");
        if(ctx.type() != null)
            Write(typeconvert(ctx.type()) + " ");
        else{
            Write("int "+ctx.identifier().ID()+ "=");
        }
        if(ctx.range().DOTDOT() != null){
            Write(ctx.range().NUM(0)+";"+ctx.identifier().ID()+"<"+ctx.range().NUM(1)+";"+ctx.identifier().ID()+"++");
        }
        else{
            Write(ctx.range().NUM(0)+";");
            if(ctx.range().vector().getText().equals("down") || ctx.range().vector().getText().equals("downTo")){
                Write(ctx.identifier().ID()+">"+ctx.range().NUM(1)+";"+ctx.identifier().ID()+"-="+ctx.range().NUM(2));
            }
            else if(ctx.range().vector().getText().equals("up") || ctx.range().vector().getText().equals("upTo")){
                Write(ctx.identifier().ID()+"<"+ctx.range().NUM(1)+";"+ctx.identifier().ID()+"+="+ctx.range().NUM(2));
            }
            else{
                System.out.println(ctx.range().vector().getText());
            }
        }
    }
	
	@Override public void exitForcondition(KotlinParser.ForconditionContext ctx) {
        Write(")");
    }
	
	@Override public void exitFor_(KotlinParser.For_Context ctx) {
        Write("\n");
    }
	
	@Override public void exitRange(KotlinParser.RangeContext ctx) { }
	
	@Override public void enterReturned(KotlinParser.ReturnedContext ctx) {
        Write(ctx.getChild(0).getText() + " " + ctx.getChild(1).getText()+";\n");
    }
	
	@Override public void enterVariable(KotlinParser.VariableContext ctx) {
        if(ctx.define() != null){
            if(ctx.define().VAL() != null)
                Write("final ");
        }
        if(ctx.type() != null){
            Write(typeconvert(ctx.type())+" ");
        }else{
            if(ctx.value() != null){
                if(ctx.value().NUM() != null)
                    Write("int ");
                else if(ctx.value().REAL() != null)
                    Write("Double ");
                else if(ctx.value().string() != null)
                    Write("String ");
            }
            else{
                Write("int ");
            }
        }

        Write(ctx.identifier().ID());
        if(ctx.EQ() != null){
            Write("=");
            if(ctx.value()!=null)
                Write(ctx.value().getText()+";\n");
            else if(ctx.expr().fcall() == null)
                Write(ctx.expr().getText()+";\n");
        }
        else
            Write(";\n");
    }
}
public class Kotlin2Java{
    public static void main(String[] args) throws IOException{
        String input = new String();
        File fo;
        try{
            File f = new File(args[0]);
            FileReader reader = new FileReader(f);
            BufferedReader buf = new BufferedReader(reader);
            Scanner scan=new Scanner(f);
            while(scan.hasNextLine()){
                input+=scan.nextLine();
                input+='\n';
            }
        }catch(IOException e){
            System.out.println(e);
        }
        try{
            fo = new File(args[1]);
        }catch(Exception e){
            fo = new File("output.java");
        }
        //Get lexer
        KotlinLexer lexer = new KotlinLexer(new ANTLRInputStream(input));
        //Get a list of matched tokens
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        //Pass Tokens to parser
        KotlinParser parser = new KotlinParser(tokens);
        //Walk parser-tree and attach our listener
        ParseTreeWalker walker = new ParseTreeWalker();
        EvalListener listener = new EvalListener(fo);

        // walk from the root of parse tree
        walker.walk(listener, parser.prog());

    }
}