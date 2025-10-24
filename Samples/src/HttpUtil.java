import java.io.IOException;
import java.net.URI;
import java.net.http.*;

public class HttpUtil {

    public static HttpResponse<String> sendGet(String url) throws IOException, InterruptedException {
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .GET()
                .build();

        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
        System.out.println("GET " + url + " -> " + response.statusCode());
        System.out.println(response.body());
        return response;
    }

    public static HttpResponse<String> sendPost(String url, String json) throws IOException, InterruptedException {
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .header("Content-Type", "application/json")
                .POST(HttpRequest.BodyPublishers.ofString(json))
                .build();

        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
        System.out.println("POST " + url + " -> " + response.statusCode());
        System.out.println(response.body());
        return response;
    }
}

