from selenium import webdriver
from selenium.webdriver.common.by import By
import re

# firefox
driver = webdriver.Firefox()
driver.get('http://202.38.93.111:10047')

# input type="text"
inputBox = driver.find_element(By.CSS_SELECTOR, 'input[type="text"]')
inputBox.send_keys(
    '1:MEUCIQC24dB6B24/LDr2O+4cifbzOEFDbkXg3hJIqTXuuvpa1QIgbzMM/F0uUmYIudtM6qEDvOpEHbtTZjSjTWMcA5zhnos= ')

# input type="submit"
# click img-fluid
driver.find_element(By.CSS_SELECTOR, 'input[type="submit"]').click()
driver.find_element(By.CLASS_NAME, 'img-fluid').click()

# <label for="captcha1">160092426831461187501631690638141489463+53014117698106737620695077701380624357 的结果是？</label>
# <input type="text" class="form-control" id="captcha1" name="captcha1" placeholder="请输入结果">
for i in range(3):
    captcha = driver.find_element(By.CSS_SELECTOR, 'label[for="captcha' + str(i + 1) + '"]').text
    result = re.search(r'(.+) 的结果是', captcha).group(1)
    ans = eval(result)
    driver.find_element(By.CSS_SELECTOR, 'input[name="captcha' + str(i + 1) + '"]').send_keys(ans)

# <button type="submit" class="btn btn-primary" id="submit">提交</button>
driver.find_element(By.ID, 'submit').click()
