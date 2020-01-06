java -jar /usr/local/lib/antlr-4.7.2-complete.jar Kotlin.g4
javac Kotlin*.java
# grun Kotlin prog -gui < input
java org.antlr.v4.gui.TestRig Kotlin prog -gui < input