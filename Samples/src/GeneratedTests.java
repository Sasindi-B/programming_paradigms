import java.net.http.HttpResponse;
import java.util.regex.Pattern;

public class GeneratedTests {
    private static void assertContains(String body, String needle, String message) {
        if (body == null || !body.contains(needle)) {
            throw new RuntimeException("Assertion failed: " + message + " (missing '" + needle + "')\nBody: " + body);
        }
    }

    private static void assertRegex(String body, String regex, String message) {
        if (body == null || !Pattern.compile(regex).matcher(body).find()) {
            throw new RuntimeException("Assertion failed: " + message + " (regex '" + regex + "' not found)\nBody: " + body);
        }
    }

    public static void testGet() throws Exception {
        HttpResponse<String> resp = HttpUtil.sendGet("http://localhost:8080/api/get");
        if (resp.statusCode() != 200) {
            throw new RuntimeException("GET /api/get expected 200, got " + resp.statusCode());
        }
        String body = resp.body();
        // Validate expected fields from GET response
        assertContains(body, "\"status\":\"success\"", "GET should include status=success");
        assertContains(body, "\"message\":\"GET request received\"", "GET should include message");
        assertRegex(body, "\\\"timestamp\\\"\\s*:\\s*\\d+", "GET should include numeric timestamp");
        System.out.println("Assertions passed: GET status/body");
    }

    /**
     * @throws Exception
     */
    public static void testPost() throws Exception {
        String payload = "{\"programming\":\"paradigms\",\"n\":assignmets}";
        HttpResponse<String> resp = HttpUtil.sendPost("http://localhost:8080/api/post", payload);
        if (resp.statusCode() != 201) {
            throw new RuntimeException("POST /api/post expected 201, got " + resp.statusCode());
        }
        String body = resp.body();
        // Validate expected fields from POST response
        assertContains(body, "\"status\":\"created\"", "POST should include status=created");
        assertContains(body, "\"message\":\"POST ok\"", "POST should include message");
        // Validate that inputs were echoed under 'received'
        assertRegex(body, "\\\"received\\\"\\s*:\\s*\\{[\\s\\S]*\\\"programming\\\"\\s*:\\s*\\\"paradigms\\\"[\\s\\S]*\\\}", "POST should echo 'programming' in received");
        assertRegex(body, "\\\"received\\\"\\s*:\\s*\\{[\\s\\S]*\\\"n\\\"\\s*:\\s*assignmets[\\s\\S]*\\\}", "POST should echo 'n' in received");
        System.out.println("Assertions passed: POST status/body/echoed inputs");
    }

    // Expected: GET -> 200 + fields, POST -> 201 + echoed inputs
    public static void main(String[] args) throws Exception {
        testGet();
        testPost();
    }
}
