use lazy_static::lazy_static;
use regex::Regex;
use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;

#[derive(Clone, Debug, Eq, PartialEq)]
enum Input {
    Value(u32),
    Wire(String),
}

#[derive(Clone, Debug, Eq, PartialEq)]
enum Gate {
    Direct {
        negated: bool,
        input: Input,
        output: String,
    },
    Or {
        left: Input,
        right: Input,
        output: String,
    },
    And {
        left: Input,
        right: Input,
        output: String,
    },
    Lshift {
        left: Input,
        right: Input,
        output: String,
    },
    Rshift {
        left: Input,
        right: Input,
        output: String,
    },
}

fn to_gate(input: &str) -> Gate {
    lazy_static! {
        static ref RE : Regex = Regex::new(r"((NOT )?((\d+)|([[:alpha:]]+)) -> ([[:alpha:]]+))|(((\d+)|([[:alpha:]]+)) (AND|OR|LSHIFT|RSHIFT) ((\d+)|([[:alpha:]]+)) -> ([[:alpha:]]+))").unwrap();
    }

    let captures = RE.captures(input).unwrap();
    if let Some(_) = captures.get(1) {
        let negated = captures.get(2).is_some();

        let input = if let Some(value) = captures.get(4) {
            Input::Value(u32::from_str(value.as_str()).unwrap())
        } else {
            Input::Wire(captures[5].to_string())
        };

        let output = captures[6].to_string();

        Gate::Direct {
            negated,
            input,
            output,
        }
    } else if let Some(_) = captures.get(7) {
        let left = if let Some(value) = captures.get(9) {
            Input::Value(u32::from_str(value.as_str()).unwrap())
        } else {
            Input::Wire(captures[10].to_string())
        };

        let right = if let Some(value) = captures.get(13) {
            Input::Value(u32::from_str(value.as_str()).unwrap())
        } else {
            Input::Wire(captures[14].to_string())
        };

        let output = captures[15].to_string();

        match captures[11].to_string().as_str() {
            "AND" => Gate::And {
                left,
                right,
                output,
            },
            "OR" => Gate::Or {
                left,
                right,
                output,
            },
            "LSHIFT" => Gate::Lshift {
                left,
                right,
                output,
            },
            "RSHIFT" => Gate::Rshift {
                left,
                right,
                output,
            },
            op => panic!("Unrecognized operation: {}", op),
        }
    } else {
        panic!("Unrecognized gate: {}", input);
    }
}

fn resolve_output(gate: &Gate) -> &String {
    match gate {
        Gate::Direct { output, .. } => output,
        Gate::And { output, .. } => output,
        Gate::Or { output, .. } => output,
        Gate::Lshift { output, .. } => output,
        Gate::Rshift { output, .. } => output,
    }
}

fn evaluation_needed(gate: &Gate, wires: &HashMap<String, u32>) -> Option<String> {
    let output = resolve_output(gate);

    if !wires.contains_key(output) {
        Some(output.clone())
    } else {
        None
    }
}

fn resolve_input(input: &Input, wires: &HashMap<String, u32>) -> Option<u32> {
    match input {
        Input::Value(v) => Some(*v),
        Input::Wire(w) => wires.get(w).map_or(None, |v| Some(*v)),
    }
}

fn evaluate(gate: &Gate, wires: &mut HashMap<String, u32>) -> bool {
    if let Some(output) = evaluation_needed(gate, wires) {
        let value = match gate {
            Gate::Direct { negated, input, .. } => resolve_input(input, wires)
                .map_or(None, |v| Some(if *negated { v ^ 0xFF_FF_FF_FF } else { v })),
            Gate::And { left, right, .. } => {
                if let (Some(left), Some(right)) =
                    (resolve_input(left, wires), resolve_input(right, wires))
                {
                    Some(left & right)
                } else {
                    None
                }
            }
            Gate::Or { left, right, .. } => {
                if let (Some(left), Some(right)) =
                    (resolve_input(left, wires), resolve_input(right, wires))
                {
                    Some(left | right)
                } else {
                    None
                }
            }
            Gate::Lshift { left, right, .. } => {
                if let (Some(left), Some(right)) =
                    (resolve_input(left, wires), resolve_input(right, wires))
                {
                    Some(left << right)
                } else {
                    None
                }
            }
            Gate::Rshift { left, right, .. } => {
                if let (Some(left), Some(right)) =
                    (resolve_input(left, wires), resolve_input(right, wires))
                {
                    Some(left >> right)
                } else {
                    None
                }
            }
        };

        if let Some(value) = value {
            wires.insert(output.clone(), value);
            return true;
        }

        return false;
    }
    true
}

fn evaluate_gates(gates: &Vec<Gate>, wires: &mut HashMap<String, u32>) {
    let mut unevaluated = gates.clone();

    while !unevaluated.is_empty() {
        let mut still_unevaluated: Vec<Gate> = Vec::new();

        for gate in &unevaluated {
            if !evaluate(gate, wires) {
                still_unevaluated.push(gate.clone())
            }
        }

        unevaluated = still_unevaluated;
    }
}

pub fn part1(input: File) -> String {
    let reader = BufReader::new(input);

    let gates: Vec<Gate> = reader
        .lines()
        .map(|l| to_gate(l.unwrap().as_str()))
        .collect();

    let mut wires = HashMap::<String, u32>::new();

    evaluate_gates(&gates, &mut wires);

    wires["a"].to_string()
}

pub fn part2(input: File) -> String {
    let reader = BufReader::new(input);

    let gates: Vec<Gate> = reader
        .lines()
        .map(|l| to_gate(l.unwrap().as_str()))
        .collect();

    let mut wires = HashMap::<String, u32>::new();

    evaluate_gates(&gates, &mut wires);

    let value_for_b = wires["a"];
    wires.clear();
    wires.insert("b".to_string(), value_for_b);

    let new_gates = gates
        .into_iter()
        .filter(|g| resolve_output(g) != "b")
        .collect();

    evaluate_gates(&new_gates, &mut wires);

    wires["a"].to_string()
}

#[test]
fn test_gate_parsing() {
    assert_eq!(
        Gate::Direct {
            negated: false,
            input: Input::Value(123),
            output: "x".to_string()
        },
        to_gate(&"123 -> x")
    );
    assert_eq!(
        Gate::Direct {
            negated: false,
            input: Input::Value(456),
            output: "y".to_string()
        },
        to_gate(&"456 -> y")
    );
    assert_eq!(
        Gate::Direct {
            negated: true,
            input: Input::Wire("x".to_string()),
            output: "h".to_string()
        },
        to_gate(&"NOT x -> h")
    );
    assert_eq!(
        Gate::Direct {
            negated: true,
            input: Input::Wire("y".to_string()),
            output: "i".to_string()
        },
        to_gate(&"NOT y -> i")
    );
    assert_eq!(
        Gate::And {
            left: Input::Wire("x".to_string()),
            right: Input::Wire("y".to_string()),
            output: "d".to_string()
        },
        to_gate(&"x AND y -> d")
    );
    assert_eq!(
        Gate::Or {
            left: Input::Wire("x".to_string()),
            right: Input::Wire("y".to_string()),
            output: "e".to_string()
        },
        to_gate(&"x OR y -> e")
    );
    assert_eq!(
        Gate::Lshift {
            left: Input::Wire("x".to_string()),
            right: Input::Value(2),
            output: "f".to_string()
        },
        to_gate(&"x LSHIFT 2 -> f")
    );
    assert_eq!(
        Gate::Rshift {
            left: Input::Wire("x".to_string()),
            right: Input::Value(2),
            output: "g".to_string()
        },
        to_gate(&"x RSHIFT 2 -> g")
    );
}
