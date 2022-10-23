# automatically generated by the FlatBuffers compiler, do not modify

# namespace: Visualisation

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class SinglePlot(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAs(cls, buf, offset=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = SinglePlot()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def GetRootAsSinglePlot(cls, buf, offset=0):
        """This method is deprecated. Please switch to GetRootAs."""
        return cls.GetRootAs(buf, offset)
    # SinglePlot
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # SinglePlot
    def Title(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.String(o + self._tab.Pos)
        return None

    # SinglePlot
    def X(self, j):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            a = self._tab.Vector(o)
            return self._tab.Get(flatbuffers.number_types.Float32Flags, a + flatbuffers.number_types.UOffsetTFlags.py_type(j * 4))
        return 0

    # SinglePlot
    def XAsNumpy(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            return self._tab.GetVectorAsNumpy(flatbuffers.number_types.Float32Flags, o)
        return 0

    # SinglePlot
    def XLength(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            return self._tab.VectorLen(o)
        return 0

    # SinglePlot
    def XIsNone(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        return o == 0

    # SinglePlot
    def Y(self, j):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(8))
        if o != 0:
            a = self._tab.Vector(o)
            return self._tab.Get(flatbuffers.number_types.Float32Flags, a + flatbuffers.number_types.UOffsetTFlags.py_type(j * 4))
        return 0

    # SinglePlot
    def YAsNumpy(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(8))
        if o != 0:
            return self._tab.GetVectorAsNumpy(flatbuffers.number_types.Float32Flags, o)
        return 0

    # SinglePlot
    def YLength(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(8))
        if o != 0:
            return self._tab.VectorLen(o)
        return 0

    # SinglePlot
    def YIsNone(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(8))
        return o == 0

def SinglePlotStart(builder): builder.StartObject(3)
def Start(builder):
    return SinglePlotStart(builder)
def SinglePlotAddTitle(builder, title): builder.PrependUOffsetTRelativeSlot(0, flatbuffers.number_types.UOffsetTFlags.py_type(title), 0)
def AddTitle(builder, title):
    return SinglePlotAddTitle(builder, title)
def SinglePlotAddX(builder, x): builder.PrependUOffsetTRelativeSlot(1, flatbuffers.number_types.UOffsetTFlags.py_type(x), 0)
def AddX(builder, x):
    return SinglePlotAddX(builder, x)
def SinglePlotStartXVector(builder, numElems): return builder.StartVector(4, numElems, 4)
def StartXVector(builder, numElems):
    return SinglePlotStartXVector(builder, numElems)
def SinglePlotAddY(builder, y): builder.PrependUOffsetTRelativeSlot(2, flatbuffers.number_types.UOffsetTFlags.py_type(y), 0)
def AddY(builder, y):
    return SinglePlotAddY(builder, y)
def SinglePlotStartYVector(builder, numElems): return builder.StartVector(4, numElems, 4)
def StartYVector(builder, numElems):
    return SinglePlotStartYVector(builder, numElems)
def SinglePlotEnd(builder): return builder.EndObject()
def End(builder):
    return SinglePlotEnd(builder)
try:
    from typing import List
except:
    pass

class SinglePlotT(object):

    # SinglePlotT
    def __init__(self):
        self.title = None  # type: str
        self.x = None  # type: List[float]
        self.y = None  # type: List[float]

    @classmethod
    def InitFromBuf(cls, buf, pos):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, 0)
        singlePlot = SinglePlot()
        singlePlot.Init(buf, pos+n)
        return cls.InitFromObj(singlePlot)

    @classmethod
    def InitFromObj(cls, singlePlot):
        x = SinglePlotT()
        x._UnPack(singlePlot)
        return x

    # SinglePlotT
    def _UnPack(self, singlePlot):
        if singlePlot is None:
            return
        self.title = singlePlot.Title()
        if not singlePlot.XIsNone():
            if np is None:
                self.x = []
                for i in range(singlePlot.XLength()):
                    self.x.append(singlePlot.X(i))
            else:
                self.x = singlePlot.XAsNumpy()
        if not singlePlot.YIsNone():
            if np is None:
                self.y = []
                for i in range(singlePlot.YLength()):
                    self.y.append(singlePlot.Y(i))
            else:
                self.y = singlePlot.YAsNumpy()

    # SinglePlotT
    def Pack(self, builder):
        if self.title is not None:
            title = builder.CreateString(self.title)
        if self.x is not None:
            if np is not None and type(self.x) is np.ndarray:
                x = builder.CreateNumpyVector(self.x)
            else:
                SinglePlotStartXVector(builder, len(self.x))
                for i in reversed(range(len(self.x))):
                    builder.PrependFloat32(self.x[i])
                x = builder.EndVector()
        if self.y is not None:
            if np is not None and type(self.y) is np.ndarray:
                y = builder.CreateNumpyVector(self.y)
            else:
                SinglePlotStartYVector(builder, len(self.y))
                for i in reversed(range(len(self.y))):
                    builder.PrependFloat32(self.y[i])
                y = builder.EndVector()
        SinglePlotStart(builder)
        if self.title is not None:
            SinglePlotAddTitle(builder, title)
        if self.x is not None:
            SinglePlotAddX(builder, x)
        if self.y is not None:
            SinglePlotAddY(builder, y)
        singlePlot = SinglePlotEnd(builder)
        return singlePlot
