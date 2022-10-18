

import serial
import serial.tools.list_ports
import time
import datetime
import threading
from tkinter import *
from tkinter.ttk import *
from tkinter.messagebox import *

 
DATA = ""		# 读取的数据
NOEND = True 	# 是否读取结束
 
 
class cbh_Serial():
	def __init__(self):
		self.ser = serial.Serial()
		self.showinterface()
 
	def on_closing(self):
		global	NOEND
		NOEND=False
		if self.ser.isOpen():
			self.thread.join()
			self.ser.close()
		self.root.destroy()
 
	# 读数据的本体
	def read_data(self,ser):
		global DATA, NOEND
		while NOEND:
			if self.ser.in_waiting :
				time.sleep(0.01)
				DATA = self.ser.read(self.ser.in_waiting)
				print(DATA)
				self.recv.insert(END, DATA.decode())
				self.recv.see(END)
			else:
				time.sleep(0.01)

	def open_seri(self,portx, bps, timeout):
		global NOEND
		ret = False
		try:
			self.ser = serial.Serial(portx, bps, timeout=timeout)
			# 判断是否成功打开
			if self.ser.isOpen():
				ret = True
				# 创建一个子线程去等待读数据
				self.thread = threading.Thread(target=self.read_data, args=(self.ser,))
				# read_data函数中会使用NOEND做判断
				NOEND = True
				self.thread.start()
		except Exception as e:
			print("error!", e) 
		return self.ser, ret
 
 
	def btn_hit(self):
		global NOEND
		if self.ser.isOpen():
			NOEND = False
			self.thread.join()
			self.ser.close()
			self.comopenbtnstr.set("打开串口")
			self.comopenflagstr.set("串口已关闭")
		else:
			# None表示等待时间为一直等待
			self.ser, ret = self.open_seri(self.cbcomportvar.get(),self.cbcombpsvar.get(), None)
			# 判断串口是否成功打开
			if ret==True:
				self.comopenbtnstr.set("关闭串口")
				self.comopenflagstr.set("串口已打开")
			else:
				showinfo(title='提示',message='打开串口失败')
 
	def showinterface(self):
		self.root = Tk()
		self.root.title("串口调试助手")
		#self.root.geometry("560x500")
 
		#screenwidth = self.root.winfo_screenwidth()
		#screenheight = self.root.winfo_screenheight()
		#alignstr = '%dx%d+%d+%d' % (width, height, (screenwidth - width) / 2, (screenheight - height) / 2)
		# 打开串口按钮
		self.comopenflagstr = StringVar()
		self.comopenflagstr.set("串口已关闭")
		self.labelname = Label(self.root,textvariable = self.comopenflagstr)
		self.comopenbtnstr = StringVar()
		self.comopenbtnstr.set("打开串口")
		self.btnopencom = Button(self.root,textvariable = self.comopenbtnstr,command = self.btn_hit)
 

		# 获取存在的端口号
		self.comlist=[]
		port_list = list(serial.tools.list_ports.comports())
		if len(port_list) == 0:
			print('无可用串口')
			self.comlist.append("无串口")
			self.btnopencom['state'] = 'disabled'
		else:
			port_list.reverse()
			for i in range(0,len(port_list)):
				plist_com = port_list[i]
				plist_com = list(port_list[i])
				self.comlist.append(plist_com[0])
 
		self.labelport = Label(self.root,text="端口")
		self.cbcomportvar = StringVar()
		self.cbport = Combobox(self.root,textvariable=self.cbcomportvar) 
		self.cbport["value"]=tuple(self.comlist)
		self.cbport.current(0)
 
		# 波特率
		self.labelbps = Label(self.root,text="波特率")
		self.cbcombpsvar = StringVar()
		self.cbbps = Combobox(self.root,textvariable=self.cbcombpsvar) 
		self.cbbps["value"]=("115200", "9600", "4800")
		self.cbbps.current(0)
 
		# 发送数据	
		self.btncomsend = Button(self.root,text = "发送",command = self.btn_sendcmd)
		self.sendx=StringVar()
		self.sendcmd=Entry(self.root,textvariable=self.sendx)
 
		# 接收数据	
		self.labelrecv = Label(self.root,text="接收的数据")
		self.recv = Text(self.root, width=60, height= 24)
		self.recv_clean = Button(self.root,text = "清空",command = self.btn_clean_recv)
		
		# 开灯
		self.led_on = Button(self.root,text = "开灯",command = self.btn_led_on)
		self.led_off = Button(self.root,text = "关灯",command = self.btn_led_off)
		
		# grid布局
		self.labelname.grid(row=0,column=0, pady=2, padx=3,sticky=W)
		self.btnopencom.grid(row=0,column=1, pady=2, padx=3,sticky=E+W)
 
		self.labelport.grid(row=1,column=0, pady=2, padx=3,sticky=W)
		self.cbport.grid(row=1,column=1, pady=2, padx=3,sticky=E+W)
 
		self.labelbps.grid(row=2,column=0, pady=2, padx=3,sticky=W)
		self.cbbps.grid(row=2,column=1, pady=2, padx=3,sticky=E+W)
		self.btncomsend.grid(row=3,column=0,pady=2, padx=3,sticky=W)
		self.sendcmd.grid(row=3,column=1,pady=2, padx=3,sticky=W+E)
 
		self.labelrecv.grid(row=4,column=0,columnspan=2,padx=3,pady=3,sticky=W)
		self.recv_clean.grid(row=4, column=1,pady=2, padx=3,sticky=W)
		self.recv.grid(row=7,column=0,columnspan=2,padx=3,pady=3,sticky=W+E)
		
		self.led_on.grid(row=8, column=0, columnspan=2, pady=0, padx=3, ipady = 20, sticky=W+E)
		self.led_off.grid(row=9, column=0, columnspan=2, pady=0, padx=3,ipady = 20, sticky=W+E)
		
		# 阻止Python GUI的大小调整
		self.root.resizable(0,0) 
		self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
		self.root.mainloop()
 
	def btn_clean_recv(self):
		 self.recv.delete(1.0, END)

	def btn_led_on(self):
		if self.ser.isOpen():
			self.ser.write(str.encode("LED3ON\r"))
		else:
			showinfo(title='提示',message='串口尚未打开')
			
	def btn_led_off(self):
		if self.ser.isOpen():
			self.ser.write(str.encode("LED3OFF\r"))
		else:
			showinfo(title='提示',message='串口尚未打开')
			
	def btn_sendcmd(self):
		if self.ser.isOpen():
			#text = b'c'
			text = self.sendx.get()
			self.ser.write(str.encode(text+"\r"))
		else:
			showinfo(title='提示',message='串口尚未打开')
 
 
if __name__ == "__main__":	 
   myserial = cbh_Serial()