import numpy as np
import math

class numpy2ccode:
    def __init__(self, f, fmt='{:Md}', Ncol=None, Nchar=128):
        """
        @param f: file
        @param fmt: format string, M replace to maximum length
             example: "{:Md}", "{:#0Mx}"
        """
        self.f = f
        self.fmt = fmt
        self.Ncol = Ncol
        self.Nchar = Nchar
        self.line_i = 0
        self.f.write('#include <stdint.h>\n')

    def _write_head(self, tn, arr, base, commit):
        typename = arr.dtype.name
        shape = ''.join(map(lambda x: '[{}]'.format(x), arr.shape))
        head = '\nconst {}_t {}{} = {{'.format(typename, tn, shape)
        self.f.write(head)
        if commit is not None:
            n = base - (len(head) - 1)  # 不含head的'\n'
            self.f.write(' '*n)
            self.f.write('  //' + commit)
        self.f.write('\n')

    def _write_tail(self):
        self.f.write('};\n')       # append end '};'

    def _write_line(self, line, fmt, commit, end):
        h = ', '.join(map(lambda x:fmt.format(x), line))
        if not end:
            h += ','
        if end and commit is not None:
            h += ' '
        if commit is not None:
            h += '  //' + commit
        h += '\n'
        self.f.write(h)

    def write_1d(self, tn, arr, fmt=None, Ncol=None, Nchar=None, commits=None):
        """
        @param Ncol: 数据列数
        @param Nchar: 每行长度限制
        @param commits: list of string, len(commits) = arr.shape[0]+1
        """
        fmt = fmt or self.fmt
        Ncol = Ncol or self.Ncol
        Nchar = Nchar or self.Nchar
        assert arr.ndim == 1
        leng = arr.shape[0]

        mcol = fmt.find(':')
        lbra = fmt.find('{')
        rbra = fmt.rfind('}')
        fmttype = fmt[rbra-1]
        fmtspec = fmt[mcol:rbra]
        pos1 = '+' in fmtspec or ' ' in fmtspec
        Mldict = {'b': math.log2,
                  'o': lambda x:math.log2(x)/3,
                  'x': lambda x:math.log2(x)/4,
                  'd': math.log10}
        if fmttype not in Mldict:
            raise ValueError
        Mlog = Mldict[fmttype]
        if arr.max() > 0:
            Mp = math.ceil(Mlog(np.abs(arr.max())))
        else:
            Mp = 0
        if arr.min() < 0:
            Ms = math.ceil(Mlog(np.abs(arr.min())))
        else:
            Ms = 0
        M = max(Mp+pos1, Ms+1)
        if '#' in fmtspec:
            M += 2
        fmt2 = fmt.replace('M', str(M))
        M2 = M + lbra + (len(fmt) - rbra - 1)  #包含前后字符

        if Ncol is None:
            Ncol = (Nchar+1)//M2
        self._write_head(tn, arr, Ncol*(M2+2)-1, commits[0] if commits else None)
        n_lines = math.ceil(leng/Ncol)  # 总行数
        # 把一维数组转文本中分布的二维数组，不包含最后一行
        arr2 = arr[:(n_lines-1)*Ncol].reshape(-1, Ncol)
        for i, line in enumerate(arr2):
            self._write_line(line, fmt2, commits[i+1] if commits else None, end=False)
        arr3 = arr[(n_lines-1)*Ncol:]  #最后一行
        self._write_line(arr3, fmt2, commits[-1] if commits else None, end=True)
        self._write_tail()
