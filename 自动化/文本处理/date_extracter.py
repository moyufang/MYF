import re

def is_valid_date(year, month, day):
	arr = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
	year = int(year)
	month = int(month)
	day = int(day)
	#print("test(valid):", year, month, day)
	if year <= 0 or year >= 10000 or \
		not (1 <= month and month <= 12) or not (1 <= day and day <= 31):
		return False
	elif year %4 != 0 or month != 2 or year %100 == 0:
		return day <= arr[month]
	else:
		return day <= 29

def is_valid_date_md(month, day):
	arr = [0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
	month = int(month)
	day = int(day)
	return (1 <= month and month <= 12) and (1 <= day <= arr[month])

def is_valid_date_ym(year, month):
	year = int(year)
	month = int(month)
	return 1 <= year and year < 10000 and 1 <= month and month <= 12

def extracter_(s):
	s = ' '+s+' '+'0'
	# 匹配非数字+数字的组合，非数字部分作为分组1，数字部分作为分组2
	pattern = re.compile(r'(\D+)(\d+)')
	# 使用 findall 查找所有匹配的子串
	matches = pattern.findall(s)
	matches = [xx for x in matches for xx in x]
	tag = [0]*len(matches)
	for i, x in enumerate(matches):
		if x.isdigit(): tag[i] |= 1
		if 'year' in x or '年' in x: tag[i] |= 2
		if 'month' in x or '月' in x: tag[i] |= 4
		if 'day' in x or '日' in x: tag[i] |= 8
	# print(matches)
	# print(tag)
	# print(len(matches))
	n = len(matches)
	y, m, d = None, None, None
	for i in range(1, n-4):
		if not (tag[i-1]&1) != 0 or not (tag[i]&2) != 0: continue
		for j in range(i+4, n):
			if (tag[j-3]&1) != 0 and (tag[j-2]&4) != 0 and \
				(tag[j-1]&1) != 0 and (tag[j]&8) != 0:
					if is_valid_date(matches[i-1], matches[j-3], matches[j-1]):
						y, m, d = matches[i-1], matches[j-3], matches[j-1]
						return y, m, d
	for i in range(4, n):
		#print("i:", i, tag[i], tag[i-2], tag[i-4])
		if (tag[i]&1) != 0 and (tag[i-2]&1) != 0 and (tag[i-4]&1) != 0:
			if is_valid_date(matches[i-4], matches[i-2], matches[i]):
				y, m, d = matches[i-4], matches[i-2], matches[i]
				return y, m, d
	return y, m, d

def extracter(s):
	y, m, d = extracter_(s)
	if y != None:
		return '%d'%int(y),'%02d'%int(m),'%02d'%int(d)

if __name__ == '__main__':
	str_lst = [
		'2024.-324234,2024年5.dafsfads月11日',
		'dafjl2jljljldsajlfjasljvlckajd2024-04.27',
		'Screenshot_2023-10-30-18-37-14-95_721f7cb5d53e9f3',
		'Screenshot_2023-11-14-21-57-52-20_721f7cb5d53e9f3'
		]
	for s in str_lst:
		print(extracter(s))