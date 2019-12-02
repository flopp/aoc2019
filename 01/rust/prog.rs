const INPUT: &str = include_str!("../input.txt");

pub fn main() {
    part1();
    part2();
}

fn mass2fuel(mass: i32) -> i32 {
    return (mass / 3) - 2;
}

fn part1() {
    let masses = INPUT.lines().filter_map(|x| x.parse::<i32>().ok());
    let fuel: i32 = masses.map(mass2fuel).sum();

    println!("part1: {}", fuel);
}

fn mass2totalfuel(mass: i32) -> i32 {
    let fuel = mass2fuel(mass);
    if fuel <= 0 {
        return 0;
    }
    return fuel + mass2totalfuel(fuel);
}

fn part2() {
    let masses = INPUT.lines().filter_map(|x| x.parse::<i32>().ok());
    let fuel: i32 = masses.map(mass2totalfuel).sum();

    println!("part2: {}", fuel);
}
