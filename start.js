// start.js

Page({

 
  data: {
    opacity: 0.4,
    disabled: true,
    threshold: 0,
    rule: 'up',
    items: [
      { name: 'up', value: 'Daytime', checked: 'ture' },
     { name: 'down', value: 'Night' },
   ]
  },

  radioChange: function (e) {
    //保存报警规则到当前页面的数据
    if (e.detail.value != "") {
      this.setData({
        rule: e.detail.value
      })
    }
    console.log(this.data.rule)
  },

  send: function () {
    var that = this
    //取得门限数据和报警规则
    var thres = this.data.threshold
    var Rule = this.data.rule

    //调用百度天气API

    const requestTask = wx.request({
      url: 'https://api.map.baidu.com/telematics/v3/weather?location=北京&output=json&ak=b2Wx5fA8GjXsm4iDLHArrVTBgfy0nHcs', //百度天气API
      header: {
        'content-type': 'application/json',
      },

      success: function (res) {
        // 利用正则字符串从百度天气API的返回数据中截出今天的温度数据
        var str = res.data.results[0].weather_data[0].date;
        var tmp1 = str.match(/实时.+/);
        var tmp2 = tmp1[0].substring(3, tmp1[0].length - 2);
        var tmp = +tmp2;

        //白天
        if (that.data.rule == "up") {
          //温度大于最适温度+5或温度小于最适温度-3报警
        if (tmp > that.data.threshold + 3 )       
          {
            wx.showModal({
              title: '警报！',
              content: '温度过高！',
              success: function (res) {
                if (res.confirm) {
                  console.log('用户点击确定')
                } else if (res.cancel) {
                  console.log('用户点击取消')
                }
              }
            })
          }
        else if ( tmp < that.data.threshold - 5)
        {
          wx.showModal({
            title: '警报！',
            content: '温度过低！',
            success: function (res) {
              if (res.confirm) {
                console.log('用户点击确定')
              } else if (res.cancel) {
                console.log('用户点击取消')
              }
            }
          })
        }
          //温度处于正常范围
          else {
           {wx.showModal({
              title: '提示～',
              content: '温度处于正常范围。',
              success: function (res) {
                if (res.confirm) {
                  console.log('用户点击确定')
                } else if (res.cancel) {
                  console.log('用户点击取消')
                }
              }
            })
          }
       }}

        //晚上
        if (that.data.rule == "down"){
          if (tmp <= that.data.threshold - 3) {
              wx.showModal({
              title: '警报！',
              content: '温度过低！',
              success: function (res) {
                if (res.confirm) {
                  console.log('用户点击确定')
                } else if (res.cancel) {
                  console.log('用户点击取消')
                }
              }
            })
          }
          else if ( tmp >= that.data.threshold + 5)
          {
            wx.showModal({
              title: '警报！',
              content: '温度过高！',
              success: function (res) {
                if (res.confirm) {
                  console.log('用户点击确定')
                } else if (res.cancel) {
                  console.log('用户点击取消')
                }
              }
            })
          }

          
          else  {
            wx.showModal({
              title: '警报！',
              content: '温度异常！',
              success: function (res) {
                if (res.confirm) {
                  console.log('用户点击确定')
                } else if (res.cancel) {
                  console.log('用户点击取消')
                }
              }
            })
          }
        }
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },

  getDataFromOneNet: function () {
    //从oneNET请求我们的绿植管家的数据
    const requestTask = wx.request({
      url: 'https://api.heclouds.com/devices/20471364/datapoints?datastream_id=Light,Temperature,Humidity&limit=30',
      header: {
        'content-type': 'application/json',
        'api-key': 'Xmj4aNDYVHaEZ3d3fsU=v4Jrh2o='
      },
      success: function (res) {
        //console.log(res.data)
        //拿到数据后保存到全局数据
        var app = getApp()
        app.globalData.temperature = res.data.data.datastreams[0]
        app.globalData.light = res.data.data.datastreams[1]
        app.globalData.humidity = res.data.data.datastreams[2]
        console.log(app.globalData.light)
        //跳转到天气页面，根据拿到的数据绘图
        wx.navigateTo({
          url: '../wifi_station/tianqi/tianqi',
        })
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },

  change: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        threshold: e.detail.value,
        opacity: 1,
        disabled: false,
      })
    } else {
      this.setData({
        threshold: 0,
        opacity: 0.4,
        disabled: true,
      })
    }
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})