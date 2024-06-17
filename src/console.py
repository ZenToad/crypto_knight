from argparse import ArgumentParser as AP
from argparse import ArgumentTypeError as ATE

def valid_ip(ip):
    parts = ip.split('.')
    if len(parts) != 4:
        raise ATE(f"{ip} is not a valid IP address")
    for part in parts:
        if not part.isdigit() or not 0 <= int(part) <= 255:
            raise ATE(f"{ip} is not a valid IP address")
    return ip


def main():
    parser = AP(description='Hack you ass off')
    parser.add_argument('--ip', type=valid_ip, help='An IPv4 address')

    args = parser.parse_args()
    print(args.ip)

if __name__ == '__main__':
    main()
