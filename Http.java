import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.*;
import java.net.InetSocketAddress;

public class Http {

    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: java Http <port>");
            System.exit(1);
        }

        int port = Integer.parseInt(args[0]);
        HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);

        server.createContext("/", new PageHandler("index.html"));

        // Add custom error handlers
        server.createContext("/error400", new ErrorResponseHandler(400, "Bad Request"));
        server.createContext("/error401", new ErrorResponseHandler(401, "Unauthorized"));
        server.createContext("/error403", new ErrorResponseHandler(403, "Forbidden"));
        server.createContext("/error404", new ErrorResponseHandler(404, "Not Found"));
        server.createContext("/error500", new ErrorResponseHandler(500, "Internal Server Error"));

        server.setExecutor(null);

        System.out.println("Server is running on port " + port);
        server.start();
    }

    static class PageHandler implements HttpHandler {
        private String fileName;

        public PageHandler(String fileName) {
            this.fileName = fileName;
        }

        @Override
        public void handle(HttpExchange exchange) throws IOException {
            InputStream is = getClass().getClassLoader().getResourceAsStream(fileName);

            if (is != null) {
                BufferedReader reader = new BufferedReader(new InputStreamReader(is));
                StringBuilder response = new StringBuilder();
                String line;
                while ((line = reader.readLine()) != null) {
                    response.append(line);
                }

                exchange.sendResponseHeaders(200, response.length());
                OutputStream os = exchange.getResponseBody();
                os.write(response.toString().getBytes());
                os.close();
            } else {
                String notFoundResponse = "File not found: " + fileName;
                exchange.sendResponseHeaders(404, notFoundResponse.length());
                OutputStream os = exchange.getResponseBody();
                os.write(notFoundResponse.getBytes());
                os.close();
            }
        }
    }

    static class ErrorResponseHandler implements HttpHandler {
        private int statusCode;
        private String statusMessage;

        public ErrorResponseHandler(int statusCode, String statusMessage) {
            this.statusCode = statusCode;
            this.statusMessage = statusMessage;
        }

        @Override
        public void handle(HttpExchange exchange) throws IOException {
            String response = statusCode + " " + statusMessage;
            exchange.sendResponseHeaders(statusCode, response.length());
            OutputStream os = exchange.getResponseBody();
            os.write(response.getBytes());
            os.close();
        }
    }
}
