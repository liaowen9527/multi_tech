


def like(str_, filter_='*'):
    if len(filter_) == 0 or filter_ == '*':
        return True

    s = filter_.startswith('*')
    e = filter_.endswith('*')

    if not s and not e:
        return str_ == filter_

    if s and e:
        key = filter_[1 : len(filter_) - 1]
        return str_.find(key) != -1

    if s:
         key = filter_[1 : len(filter_)]
         return str_.endswith(key)

    key = filter_[0 : len(filter_) - 1]
    return str_.startswith(key)


if __name__ == "__main__":
    print(like('qwe', '*qw'))
    print(like('qwe', 'qw*'))