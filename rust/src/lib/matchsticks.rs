use std::fs::File;
use std::io::{BufRead, BufReader};

use nom::branch::alt;
use nom::bytes::streaming::{is_not, take};
use nom::character::streaming::char;
use nom::combinator::{map, map_opt, map_res, value, verify};
use nom::error::{FromExternalError, ParseError};
use nom::multi::fold_many0;
use nom::sequence::{delimited, preceded};
use nom::{IResult, Parser};

fn parse_unicode<'a, E>(input: &'a str) -> IResult<&'a str, char, E>
where
    E: ParseError<&'a str> + FromExternalError<&'a str, std::num::ParseIntError>,
{
    let parse_hex = take(2usize);

    let parse_delimited_hex = preceded(char('x'), parse_hex);

    let parse_u8 = map_res(parse_delimited_hex, move |hex| u8::from_str_radix(hex, 16));

    map_opt(parse_u8, |_| Some('.')).parse(input) // The input contains sequences which don't map
                                                  // to ascii
}

fn parse_escaped_char<'a, E>(input: &'a str) -> IResult<&'a str, char, E>
where
    E: ParseError<&'a str> + FromExternalError<&'a str, std::num::ParseIntError>,
{
    preceded(
        char('\\'),
        alt((
            parse_unicode,
            value('\\', char('\\')),
            value('"', char('"')),
        )),
    )
    .parse(input)
}

fn parse_literal<'a, E: ParseError<&'a str>>(input: &'a str) -> IResult<&'a str, &'a str, E> {
    let not_quote_slash = is_not("\"\\");
    verify(not_quote_slash, |s: &str| !s.is_empty()).parse(input)
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum StringFragment<'a> {
    Literal(&'a str),
    EscapedChar(char),
}

fn parse_fragment<'a, E>(input: &'a str) -> IResult<&'a str, StringFragment<'a>, E>
where
    E: ParseError<&'a str> + FromExternalError<&'a str, std::num::ParseIntError>,
{
    alt((
        map(parse_literal, StringFragment::Literal),
        map(parse_escaped_char, StringFragment::EscapedChar),
    ))
    .parse(input)
}

fn parse_string<'a, E>(input: &'a str) -> IResult<&'a str, String, E>
where
    E: ParseError<&'a str> + FromExternalError<&'a str, std::num::ParseIntError>,
{
    let build_string = fold_many0(parse_fragment, String::new, |mut string, fragment| {
        match fragment {
            StringFragment::Literal(s) => string.push_str(s),
            StringFragment::EscapedChar(c) => string.push(c),
        }
        string
    });

    delimited(char('"'), build_string, char('"')).parse(input)
}

pub fn part1(input: File) -> String {
    let reader = BufReader::new(input);

    let difference: usize = reader
        .lines()
        .map(|l| {
            let line = l.unwrap();

            let parsed_result = parse_string::<()>(&line).unwrap();

            let parsed = parsed_result.1;

            line.len() - parsed.len()
        })
        .sum();

    difference.to_string()
}

pub fn part2(input: File) -> String {
    let reader = BufReader::new(input);

    let difference: usize = reader
        .lines()
        .map(|l| {
            let line = l.unwrap();

            let mut encoded = line.clone();

            encoded = encoded.replace('\\', "\\\\");
            encoded = "\"".to_string() + &encoded.replace('\"', "\\\"") + "\"";

            encoded.len() - line.len()
        })
        .sum();

    difference.to_string()
}

#[test]
fn test_samples() {
    let empty = "\"\"";
    assert_eq!(empty.len(), 2);
    let empty_result = parse_string::<()>(&empty);
    assert_eq!(empty_result.unwrap().1.len(), 0);

    let quote = "\"\\\"\"";
    assert_eq!(quote.len(), 4);
    let quote_result = parse_string::<()>(&quote);
    assert_eq!(quote_result.unwrap().1.len(), 1);

    let backslash = "\"\\\\\"";
    assert_eq!(backslash.len(), 4);
    let backslash_result = parse_string::<()>(&backslash);
    assert_eq!(backslash_result.unwrap().1.len(), 1);

    let apostrophe = "\"\\x27\"";
    assert_eq!(apostrophe.len(), 6);
    let apostrophe_result = parse_string::<()>(&apostrophe);
    assert_eq!(apostrophe_result.unwrap().1.len(), 1);

    let abc = "\"abc\"";
    assert_eq!(abc.len(), 5);
    let abc_result = parse_string::<()>(&abc);
    assert_eq!(abc_result.unwrap().1.len(), 3);

    let aaa = "\"aaa\\\"aaa\"";
    assert_eq!(aaa.len(), 10);
    let aaa_result = parse_string::<()>(&aaa);
    assert_eq!(aaa_result.unwrap().1.len(), 7);
}
