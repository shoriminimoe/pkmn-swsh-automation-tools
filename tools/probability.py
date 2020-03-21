from fractions import Fraction
import math

import click


BASE_SHINY_RATE = Fraction(1, 4096)
SHINY_CHARM_RATE = 3 * BASE_SHINY_RATE
MASUDA_RATE = 6 * BASE_SHINY_RATE


@click.command()
@click.option('--shiny-charm/--no-shiny-charm', default=False,
              help='Calculate probability including the shiny charm')
@click.option('--masuda/--no-masuda', default=False,
              help='Calculate probability using the masuda method')
def main(shiny_charm, masuda):
    """Calculate the probability of encountering a shiny pokemon"""
    shiny_rate = Fraction(0)
    if shiny_charm:
        shiny_rate += SHINY_CHARM_RATE
    if masuda:
        shiny_rate += MASUDA_RATE
    if shiny_rate == 0:
        shiny_rate = BASE_SHINY_RATE

    rate_str = f'Your shiny rate is : {shiny_rate}'
    if shiny_rate.numerator != 1:
        rounded_denominator = round(shiny_rate.denominator
                                    / shiny_rate.numerator)
        rate_str += f' (~1/{rounded_denominator})'
    print(rate_str)
    for i in range(50, 100, 10):
        print_required_encounters(shiny_rate, i)


def print_required_encounters(probability, confidence):
    """
    Calculate and print number of trials required to encounter an event at
    least once for a given probability and confidence level. Assumes
    independent events.
    """
    confidence_pct = Fraction(confidence, 100)
    n = math.ceil(math.log1p(-confidence_pct) / math.log1p(-probability))
    print(f'{confidence}% chance of shiny after {n} encounters')


if __name__ == '__main__':
    main()
