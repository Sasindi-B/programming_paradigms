import java.io.*;
import java.net.http.*;
import java.net.*;
public class GeneratedTests {
    public static void testGet() throws Exception { HttpUtil.sendGet("https://jsonplaceholder.typicode.com/posts/1"); }
    public static void testPost() throws Exception { HttpUtil.sendPost("https://jsonplaceholder.typicode.com/posts", "{}"); }
    // Assertion: "status == 200"
    public static void main(String[] args) throws Exception {
        testGet();
        testPost();
    }
}
