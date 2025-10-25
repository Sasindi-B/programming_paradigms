import org.junit.jupiter.api.*;
import java.net.http.*;
import java.net.URI;

public class GeneratedTests {
    private static final HttpClient client = HttpClient.newHttpClient();
    private static final String BASE_URL = "http://localhost:8080";
    private static final String user = "admin";
    private static final String id = "42";

    @Test
    public void getUser() throws Exception {
        String method = null;
        String url = null;
        String body = null;
        int expectedStatus = 200;
        HttpRequest.Builder builder = HttpRequest.newBuilder();
        builder.header("Content-Type", "application/json");
        builder.header("X-App", "TestLangDemo");
        method = "GET";
        url = BASE_URL + "/api/users/" + id + "";
        expectedStatus = 200;
        if (url != null) builder.uri(URI.create(url));
        if ("GET".equalsIgnoreCase(method)) {
            builder = builder.GET();
        } else if (body != null) {
            builder = builder.method(method, HttpRequest.BodyPublishers.ofString(body));
        } else {
            builder = builder.method(method, HttpRequest.BodyPublishers.noBody());
        }
        HttpRequest request = builder.build();
        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
        Assertions.assertEquals(expectedStatus, response.statusCode());
        String bodyStr = response.body();
    }

    @Test
    public void createUser() throws Exception {
        String method = null;
        String url = null;
        String body = null;
        int expectedStatus = 200;
        HttpRequest.Builder builder = HttpRequest.newBuilder();
        builder.header("Content-Type", "application/json");
        builder.header("X-App", "TestLangDemo");
        method = "POST";
        url = BASE_URL + "/api/post";
        body = "{\"hello\":\"world\",\"n\":123}";
        expectedStatus = 201;
        if (url != null) builder.uri(URI.create(url));
        if ("GET".equalsIgnoreCase(method)) {
            builder = builder.GET();
        } else if (body != null) {
            builder = builder.method(method, HttpRequest.BodyPublishers.ofString(body));
        } else {
            builder = builder.method(method, HttpRequest.BodyPublishers.noBody());
        }
        HttpRequest request = builder.build();
        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
        Assertions.assertEquals(expectedStatus, response.statusCode());
        String bodyStr = response.body();
        Assertions.assertTrue(bodyStr.contains("\"message\":\"POST ok\""), "body.message == 'POST ok'");
        Assertions.assertTrue(bodyStr.contains("\"received\""), "body has field received");
        Assertions.assertTrue(bodyStr.contains("programming"), "body.received contains 'programming'");
        Assertions.assertTrue(bodyStr.contains("\"received\""), "body has field received");
        Assertions.assertTrue(bodyStr.contains("paradigms"), "body.received contains 'paradigms'");
        Assertions.assertTrue(bodyStr.contains("\"received\""), "body has field received");
        Assertions.assertTrue(bodyStr.contains("assignments"), "body.received contains 'assignments'");
    }
}
