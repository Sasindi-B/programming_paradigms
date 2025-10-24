package com.testlang.backend.controller;

import org.springframework.web.bind.annotation.*;
import java.util.*;

@RestController
@RequestMapping("/api")
public class TestApiController {

    // Simple GET for DSL assertions
    @GetMapping("/get")
    public Map<String, Object> getExample() {
        Map<String, Object> res = new LinkedHashMap<>();
        res.put("status", "success");
        res.put("message", "GET request received");
        res.put("timestamp", System.currentTimeMillis());
        return res;
    }

    // POST that echoes request
    @PostMapping("/post")
    @ResponseStatus(org.springframework.http.HttpStatus.CREATED)
    public Map<String, Object> postExample(@RequestBody(required = false) Map<String, Object> body) {
        Map<String, Object> res = new LinkedHashMap<>();
        res.put("status", "created");
        res.put("received", body);
        res.put("message", "POST ok");
        return res;
    }

    // PUT that updates a value
    @PutMapping("/update/{id}")
    public Map<String, Object> putExample(@PathVariable int id, @RequestBody Map<String, Object> body) {
        Map<String, Object> res = new LinkedHashMap<>();
        res.put("status", "updated");
        res.put("id", id);
        res.put("newData", body);
        return res;
    }

    // DELETE returning no content
    @DeleteMapping("/delete/{id}")
    @ResponseStatus(org.springframework.http.HttpStatus.NO_CONTENT)
    public void deleteExample(@PathVariable int id) {}

    // 404 handler for DSL negative tests
    @GetMapping("/missing")
    @ResponseStatus(org.springframework.http.HttpStatus.NOT_FOUND)
    public Map<String, Object> notFound() {
        Map<String, Object> res = new LinkedHashMap<>();
        res.put("error", "Resource not found");
        return res;
    }
}

