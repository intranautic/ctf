"""Small local bridge to the installed ida_mcp tools."""
import asyncio
import json
import sys
from pathlib import Path
from ida_mcp import mcp

target = str(Path("crackme.exe").resolve())
mcp.open_database(target)
code = Path(sys.argv[1]).read_text(encoding="utf-8")
result = asyncio.run(mcp.execute_python(code))
print(json.dumps(result, indent=2, default=str))
