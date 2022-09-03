class numpy2ccode:
    def __init__(self, fn, tn, arr, commit=None):
        """
        @param fn: filename
        @param tn: tablename
        @param arr: array
        @param commit: list of string, len(commit) = arr.shape[0]+1
        """
        self.fn = fn
        self.tn = tn
        self.arr = arr
        self.f = None
        self.commit = commit
        self.line_i = 0

    def _big_byteorder(self):
        dtype2 = self.arr.dtype.newbyteorder('>')
        self.arr = self.arr.astype(dtype2)

    def _write_head(self, Ncol=0):
        self.f.write('#include <stdint.h>\n\n')
        name = self.arr.dtype.name
        shape = ''.join(map(lambda x: '[{}]'.format(x), self.arr.shape))
        head = 'const {}_t {}{} = {{'.format(name, self.tn, shape)
        self.f.write(head)
        if Ncol:
            n = self._wide()*Ncol - len(head) - 1
            self.f.write(' '*n)
        self._write_commit()
        self.f.write('\n')

    def _write_tail(self):
        self.f.close()
        # re-open, str mode unsupport cur/end-relative seeks
        self.f = open(self.fn, 'rb+')
        self.f.seek(-2, 2)        # remove last ',\n'
        if self.commit:
            self.f.seek(-len(self.commit[-1])-4, 1)
        self.f.write(b'};')       # append end '};'
        self.f.close()

    def _write_line(self, line):
        nhex = self.arr.itemsize*2
        h = line.tobytes().hex()
        h = ['0x'+h[i:(i+nhex)]+',' for i in range(0, len(h), nhex)]
        h = ' '.join(h)
        self.f.write(h)
        self._write_commit()
        self.f.write('\n')

    def _write_commit(self):
        if self.commit:
            h = '  //' + self.commit[self.line_i]
            self.f.write(h)
        self.line_i += 1

    def _wide(self):
        nhex = self.arr.itemsize*2
        return nhex + 4

    def conv_1d(self, Ncol=None, Nchar=128):
        assert self.arr.ndim == 1
        leng = self.arr.shape[0]
        self._big_byteorder()
        self.f = open(self.fn, 'w')
        if Ncol is None:
            Ncol = (Nchar+1)//self._wide()
        self._write_head(Ncol)
        arr2 = self.arr[:(leng//Ncol)*Ncol].reshape(-1, Ncol)
        for line in arr2:
            self._write_line(line)
        arr3 = self.arr[(leng//Ncol)*Ncol:]
        if arr3.size > 0:
            self._write_line(arr3)
        self._write_tail()

    def conv_nd(self):
        self._big_byteorder()
        self.f = open(self.fn, 'w')
        self._write_head()
        self.f.arr.reshape()
        for line in self.arr:
            self._write_line(line)
        self._write_tail()
