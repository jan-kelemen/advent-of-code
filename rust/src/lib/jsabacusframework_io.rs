use serde_json::Value;
use std::fs::File;
use std::io::BufReader;

fn all_numbers(value: &Value) -> i64 {
    if let Value::Number(n) = value {
        if let Some(x) = n.as_i64() {
            return x;
        }
    } else if let Value::Array(values) = value {
        return values.iter().map(all_numbers).sum();
    } else if let Value::Object(objects) = value {
        return objects.values().map(all_numbers).sum();
    }

    0
}

fn is_red(value: &Value) -> bool {
    if let Value::String(s) = value {
        return s == "red";
    }

    false
}

fn ignore_red_children(value: &Value) -> i64 {
    if let Value::Number(n) = value {
        if let Some(x) = n.as_i64() {
            return x;
        }
    } else if let Value::Array(values) = value {
        return values.iter().map(ignore_red_children).sum();
    } else if let Value::Object(objects) = value {
        if objects.values().any(is_red) {
            return 0;
        }

        return objects.values().map(ignore_red_children).sum();
    }

    0
}

pub fn part1(input: File) -> String {
    let value: Value;
    let reader = BufReader::new(input);
    match serde_json::from_reader(reader) {
        Ok(v) => value = v,
        Err(error) => panic!("Problem reading input file: {:?}", error),
    };

    all_numbers(&value).to_string()
}

pub fn part2(input: File) -> String {
    let value: Value;
    let reader = BufReader::new(input);
    match serde_json::from_reader(reader) {
        Ok(v) => value = v,
        Err(error) => panic!("Problem reading input file: {:?}", error),
    };

    ignore_red_children(&value).to_string()
}

#[test]
fn test_all_numbers() {
    assert_eq!(all_numbers(&serde_json::from_str("[1,2,3]").unwrap()), 6);
    assert_eq!(
        all_numbers(&serde_json::from_str("{\"a\":2,\"b\":4}").unwrap()),
        6
    );
    assert_eq!(
        all_numbers(&serde_json::from_str("{\"a\":{\"b\":4},\"c\":-1}").unwrap()),
        3
    );
    assert_eq!(all_numbers(&serde_json::from_str("[[[3]]]").unwrap()), 3);
    assert_eq!(
        all_numbers(&serde_json::from_str("{\"a\":[-1,1]}").unwrap()),
        0
    );
    assert_eq!(
        all_numbers(&serde_json::from_str("[-1,{\"a\":1}]").unwrap()),
        0
    );
    assert_eq!(all_numbers(&serde_json::from_str("[]").unwrap()), 0);
    assert_eq!(all_numbers(&serde_json::from_str("{}").unwrap()), 0);
}

#[test]
fn test_ignore_red_children() {
    assert_eq!(
        ignore_red_children(&serde_json::from_str("[1,2,3]").unwrap()),
        6
    );
    assert_eq!(
        ignore_red_children(&serde_json::from_str("{\"a\":2,\"b\":4}").unwrap()),
        6
    );
    assert_eq!(
        ignore_red_children(&serde_json::from_str("{\"a\":{\"b\":4},\"c\":-1}").unwrap()),
        3
    );
    assert_eq!(
        ignore_red_children(&serde_json::from_str("[[[3]]]").unwrap()),
        3
    );
    assert_eq!(
        ignore_red_children(&serde_json::from_str("{\"a\":[-1,1]}").unwrap()),
        0
    );
    assert_eq!(
        ignore_red_children(&serde_json::from_str("[-1,{\"a\":1}]").unwrap()),
        0
    );
    assert_eq!(ignore_red_children(&serde_json::from_str("[]").unwrap()), 0);
    assert_eq!(ignore_red_children(&serde_json::from_str("{}").unwrap()), 0);
    assert_eq!(
        ignore_red_children(&serde_json::from_str("[1,{\"c\":\"red\",\"b\":2},3]").unwrap()),
        4
    );
    assert_eq!(
        ignore_red_children(
            &serde_json::from_str("{\"d\":\"red\",\"e\":[1,2,3,4],\"f\":5}").unwrap()
        ),
        0
    );
    assert_eq!(
        ignore_red_children(&serde_json::from_str("[1,\"red\",5]").unwrap()),
        6
    );
}
