import java.io.FileWriter;
import java.io.IOException;

public class Codegen {
    public static void generate(String content) {
        try (FileWriter writer = new FileWriter("Samples/src/GeneratedTests.java", true)) {
            writer.write(content + "\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
