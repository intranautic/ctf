#!/usr/bin/env python3
import idaapi
import idc
import idautils
import re

class const_parser:
  @staticmethod
  def parse(header: str, log: bool=False) -> str:
    buffer = [
      [i.strip() for i in const.replace('\t', ' ').split(' ') if i][1::1]
        for const in re.sub('//.*?\n|/\*.*?\*/', '',
          open(header, 'r').read(),
          flags=re.S,
        ).split('\n') if const.startswith("#define")
    ]
    consts = {}
    for const in buffer:
      if len(const) >= 2:
        try:
          name, value = const[0], eval(
            ''.join([c for c in ''.join(const[1:]).strip() if c not in 'ulUL'])
          )
        except:
          if log:
            print("Failed to parse symbolic constant: %s" % name)
        consts[name] = value
    return consts

  @staticmethod
  def register(name: str, consts) -> int:
    eid = idc.add_enum(-1, name, idaapi.hex_flag())
    if not eid:
      print("Could not construct new enumeration!")
      return -1
    for value, name in enumerate(consts):
      idc.add_enum_member(eid, name, value, -1)
    return eid

#for value, name in enumerate(consts):
#  print('%s: %#lx' % (name, value))
