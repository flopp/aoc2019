const INPUT: &str = include_str!("../input.txt");

fn is_valid_password1(candidate: u32) -> bool {
    let mut c = candidate;
    let mut has_duplicate = false;

    let mut last_digit = 10;
    while c > 0 {
        let digit = c % 10;
        c = c / 10;
        
        if digit == last_digit {
            has_duplicate = true;
        } else if digit > last_digit {
            return false;
        }

        last_digit = digit;
    }

    return has_duplicate;
}

fn is_valid_password2(candidate: u32) -> bool {
    let mut c = candidate;
    let mut has_duplicate = false;
    let mut duplicate_span = 0;

    let mut last_digit = 10;
    while c > 0 {
        let digit = c % 10;
        c = c / 10;
        
        if digit == last_digit {
            duplicate_span += 1;
        } else if digit > last_digit {
            return false;
        } else {
            if duplicate_span == 2 {
                has_duplicate = true;
            }
            duplicate_span = 1
        }

        last_digit = digit;
    }

    return has_duplicate || (duplicate_span == 2);
}

pub fn main() {
    let mut split = INPUT.split("-");
    let range_from: u32 = split.next().unwrap().trim().parse().unwrap();
    let range_to: u32 = split.next().unwrap().trim().parse().unwrap();

    let mut count1 = 0;
    let mut count2 = 0;
    for candidate in range_from..=range_to {
        if is_valid_password1(candidate) {
            count1 += 1;
        }
        if is_valid_password2(candidate) {
            count2 += 1;
        }
    }

    println!("part1: {}", count1);
    println!("part2: {}", count2);
}
