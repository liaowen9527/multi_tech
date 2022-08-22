import datetime
import json

#precision: millisecond
precision = 1000

def _object_hook(dct):
    if '$date' in dct:
        return datetime.datetime.fromtimestamp(dct['$date'] / precision)
    return dct

def _default(o):
    if isinstance(o, datetime.datetime):
        f = o.timestamp()
        d = int(f) * precision + int(f * precision % precision)
        return {'$date': d}
    else:
        return o

def dumps(obj, *, skipkeys=False, ensure_ascii=True, check_circular=True,
          allow_nan=True, cls=None, indent=None, separators=None,
          default=_default, sort_keys=False, **kw):
    return json.dumps(obj, skipkeys=skipkeys, ensure_ascii=ensure_ascii, 
                      check_circular=check_circular, allow_nan=allow_nan,
                      cls=cls, indent=indent, separators=separators,
                      default=default, sort_keys=sort_keys, **kw) 

def loads(s, *, encoding=None, cls=None, object_hook=_object_hook,
          parse_float=None, parse_int=None, parse_constant=None,
          object_pairs_hook=None, **kw):
    return json.loads(s, encoding=encoding, cls=cls, object_hook=object_hook,
                      parse_float=parse_float, parse_int=parse_int,
                      parse_constant=parse_constant,
                      object_pairs_hook=object_pairs_hook, **kw) 

if __name__ == "__main__":
    t = datetime.datetime(2019, 1, 1, 13, 24, 54, 145678)
    d = {'11': 'vv', 'aaa' : t}
    j = dumps(d)
    d2 = loads(j)
    j2 = dumps(d2)

    print(j)
    print(j2)
    print(d)
    print(d2)
    