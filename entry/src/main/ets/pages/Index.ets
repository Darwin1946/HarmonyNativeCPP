import sortModule from 'libugc.so';
import hilog from '@ohos.hilog';

@Entry
@Component
struct SortPage {
  // 保持原有状态变量不变
  @State inputStr: string = '2,4,1,334,21';
  @State sortType: string = 'asc';
  @State resultStr: string = '';

  private async onSortClick() {
    // 保持原有逻辑不变
    try {
      hilog.info(0,"","[INFO] 当前输入为:"+this.inputStr)
      const numStrArray = this.inputStr.split(',').map(item => item.trim());
      const numArray: number[] = [];
      for (const str of numStrArray) {
        const num = Number(str);
        if (isNaN(num)) {
          throw new Error(`输入的"${str}"不是有效数字`);
        }
        numArray.push(num);
      }

      const sortedArray = sortModule.QuickSort(numArray, this.sortType);
      this.resultStr = sortedArray.join(', ');
    } catch (e) {
      this.resultStr = `排序失败：${(e as Error).message}`;
    }
  }

  build() {
    Column() {
      // 标题（不变）
      Text("数组排序工具（UGC QSort原生实现）")
        .fontSize(20)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 20 });

      // 输入框（不变）
      TextInput({ placeholder: "请输入逗号分隔的数字，如2,4,1,334,21", text: this.inputStr })
        .width('90%')
        .height(60)
        .border({ width: 1, color: '#ccc', radius: 8 })
        .padding(10)
        .margin({ bottom: 15 })
        .onChange((value: string) => {
          this.inputStr = value; // 将输入框的内容赋值给inputStr
        })

      // 排序类型选择：修正marginRight为margin({ right: xxx })
      Row() {
        Text("排序类型：")
          .fontSize(16)
          .margin({ right: 10 }); // ✅ 正确：设置右侧margin为10

        // 递增单选按钮
        Radio({ value: 'asc', group: 'sortType' })
          .checked(this.sortType === 'asc')
          .onChange(() => {
            this.sortType = 'asc';
          })
        Text("递增")
          .fontSize(16)
          .margin({ right: 20 }); // ✅ 正确：设置右侧margin为20

        // 递减单选按钮
        Radio({ value: 'desc', group: 'sortType' })
          .checked(this.sortType === 'desc')
          .onChange(() => {
            this.sortType = 'desc';
          })
        Text("递减")
          .fontSize(16);
      }
      .width('90%')
      .margin({ bottom: 20 });

      // 排序按钮（不变）
      Button("点击排序")
        .width('90%')
        .height(50)
        .backgroundColor('#007dff')
        .fontColor('#fff')
        .borderRadius(8)
        .onClick(() => {
          this.onSortClick();
        })
        .margin({ bottom: 20 });

      // 结果展示（不变）
      Text("排序结果：")
        .fontSize(16)
        .fontWeight(FontWeight.Medium)
        .width('90%')
        .margin({ bottom: 5 });

      Text(this.resultStr)
        .fontSize(16)
        .width('90%')
        .padding(10)
        .border({ width: 1, color: '#eee', radius: 8 })
        .backgroundColor('#f9f9f9');
    }
    .width('100%')
    .height('100%')
    .padding(20)
    .justifyContent(FlexAlign.Start);
  }
}